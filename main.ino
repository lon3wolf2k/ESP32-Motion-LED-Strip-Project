#include <Adafruit_NeoPixel.h>

// ===============================================
// CONFIGURATION
// ===============================================
#define PIR_PIN        27      // PIR OUT pin
#define LED_PIN         5      // WS2812 DATA pin
#define NUM_LEDS       89

// Brightness control
const uint8_t MAX_BRIGHTNESS = 180;       // Peak brightness during fade-in
const uint8_t FADE_STEP = 4;              // Brightness change per step
const unsigned long FADE_INTERVAL = 15;   // Time between brightness steps (ms)
const unsigned long ON_TIME = 10000UL;    // Stay on for 10 seconds after motion

// Color breathing (white <-> red) while motion is active
const unsigned long COLOR_INTERVAL = 20;  // Time between color steps (ms)
const uint8_t COLOR_STEP = 3;             // Blend change per step (0-255)

// ===============================================

Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// State tracking
unsigned long lastMotionTime = 0;
bool targetOn = false;
uint8_t currentBrightness = 0;
unsigned long lastFadeUpdate = 0;
unsigned long lastColorUpdate = 0;

// Color blend (0 = white, 255 = red)
uint8_t colorBlend = 0;
int8_t colorDir = 1;

// Base colors
const uint8_t WHITE_R = 255;
const uint8_t WHITE_G = 255;
const uint8_t WHITE_B = 200;

const uint8_t RED_R = 255;
const uint8_t RED_G = 0;
const uint8_t RED_B = 0;

void setup() {
  Serial.begin(115200);
  delay(300);

  pinMode(PIR_PIN, INPUT);

  strip.begin();
  strip.setBrightness(0);   // Start off
  applyFrame();

  Serial.println("ESP32 PIR + White/Red Fade Ready");
}

void loop() {
  unsigned long now = millis();
  int motion = digitalRead(PIR_PIN);

  if (motion == HIGH) {
    lastMotionTime = now;
    targetOn = true;
  }

  if (targetOn && (now - lastMotionTime > ON_TIME)) {
    targetOn = false;
  }

  bool brightnessChanged = updateFade(now);
  bool colorChanged = updateColor(now);

  if (brightnessChanged || colorChanged) {
    applyFrame();
  }
}

// Smoothly adjust brightness toward the target state
bool updateFade(unsigned long now) {
  if (now - lastFadeUpdate < FADE_INTERVAL) return false;
  lastFadeUpdate = now;

  uint8_t targetBrightness = targetOn ? MAX_BRIGHTNESS : 0;

  if (currentBrightness == targetBrightness) return false;

  if (currentBrightness < targetBrightness) {
    currentBrightness = min<uint8_t>(currentBrightness + FADE_STEP, targetBrightness);
  } else {
    currentBrightness = (currentBrightness > FADE_STEP) ? currentBrightness - FADE_STEP : 0;
    if (currentBrightness < targetBrightness) currentBrightness = targetBrightness;
  }

  return true;
}

// Update white <-> red blend while motion is active
bool updateColor(unsigned long now) {
  if (!targetOn) return false;
  if (now - lastColorUpdate < COLOR_INTERVAL) return false;
  lastColorUpdate = now;

  uint8_t nextBlend = colorBlend;
  if (colorDir > 0) {
    nextBlend = (colorBlend + COLOR_STEP >= 255) ? 255 : colorBlend + COLOR_STEP;
    if (nextBlend == 255) colorDir = -1;
  } else {
    nextBlend = (colorBlend < COLOR_STEP) ? 0 : colorBlend - COLOR_STEP;
    if (nextBlend == 0) colorDir = 1;
  }

  if (nextBlend == colorBlend) return false;
  colorBlend = nextBlend;
  return true;
}

// Apply the current brightness and color blend to the whole strip
void applyFrame() {
  uint8_t rBlend = ((WHITE_R * (255 - colorBlend)) + (RED_R * colorBlend)) / 255;
  uint8_t gBlend = ((WHITE_G * (255 - colorBlend)) + (RED_G * colorBlend)) / 255;
  uint8_t bBlend = ((WHITE_B * (255 - colorBlend)) + (RED_B * colorBlend)) / 255;

  uint8_t r = (rBlend * currentBrightness) / MAX_BRIGHTNESS;
  uint8_t g = (gBlend * currentBrightness) / MAX_BRIGHTNESS;
  uint8_t b = (bBlend * currentBrightness) / MAX_BRIGHTNESS;

  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, r, g, b);
  }

  strip.setBrightness(currentBrightness);
  strip.show();
}
