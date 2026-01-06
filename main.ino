
#include <Adafruit_NeoPixel.h>

// ===============================================
// CONFIGURATION
// ===============================================
#define PIR_PIN        27      // PIR OUT pin
#define LED_PIN         5      // WS2812 DATA pin
#define NUM_LEDS       89

// Brightness is controlled in code for smooth fades
const uint8_t MAX_BRIGHTNESS = 180;      // Peak brightness during fade-in
const uint8_t FADE_STEP = 4;             // Brightness change per step
const unsigned long FADE_INTERVAL = 15;  // Time between brightness steps (ms)
const unsigned long ON_TIME = 10000UL;   // Stay on for 10 seconds after motion

// ===============================================

Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// State tracking
unsigned long lastMotionTime = 0;
bool targetOn = false;
uint8_t currentBrightness = 0;
unsigned long lastFadeUpdate = 0;

// Single-zone color (soft white)
const uint8_t BASE_R = 255;
const uint8_t BASE_G = 255;
const uint8_t BASE_B = 200;

void setup() {
  Serial.begin(115200);
  delay(300);

  pinMode(PIR_PIN, INPUT);

  strip.begin();
  strip.setBrightness(0);   // Start off
  applyBrightness();

  Serial.println("ESP32 PIR + Fade In/Out Ready");
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

  updateFade(now);
}

// Smoothly adjust brightness toward the target state
void updateFade(unsigned long now) {
  if (now - lastFadeUpdate < FADE_INTERVAL) return;
  lastFadeUpdate = now;

  uint8_t targetBrightness = targetOn ? MAX_BRIGHTNESS : 0;

  if (currentBrightness == targetBrightness) return;

  if (currentBrightness < targetBrightness) {
    currentBrightness = min<uint8_t>(currentBrightness + FADE_STEP, targetBrightness);
  } else {
    currentBrightness = (currentBrightness > FADE_STEP) ? currentBrightness - FADE_STEP : 0;
    if (currentBrightness < targetBrightness) currentBrightness = targetBrightness;
  }

  applyBrightness();
}

// Apply the current brightness to the whole strip using the base color
void applyBrightness() {
  uint8_t r = (BASE_R * currentBrightness) / MAX_BRIGHTNESS;
  uint8_t g = (BASE_G * currentBrightness) / MAX_BRIGHTNESS;
  uint8_t b = (BASE_B * currentBrightness) / MAX_BRIGHTNESS;

  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, r, g, b);
  }

  strip.setBrightness(currentBrightness);
  strip.show();
}
