#include <Adafruit_NeoPixel.h>

// ========================
// CONFIGURATION
// ========================
#define LED_PIN     5
#define PIR_PIN     27
#define NUM_LEDS    47
#define BRIGHTNESS  150

// Zones
#define ZONE_A_START 0
#define ZONE_A_END   19   // 20 LEDs
#define ZONE_B_START 20
#define ZONE_B_END   46   // 27 LEDs

// PIR settings
#define PIR_DEBOUNCE_MS 200
#define ACTIVE_TIME_MS  10000    // <-- OFF after 10 seconds

// Flash Zone A settings
#define FLASH_SPEED_MS 600
bool flashState = false;
unsigned long lastFlashTime = 0;

// Knight Rider Zone B settings
#define KR_SPEED_MS 20
#define KR_TAIL 8
int krPos = ZONE_B_START;
int krDir = 1;
unsigned long lastKRTime = 0;

// PIR tracking
bool motionActive = false;
unsigned long lastMotionTime = 0;
unsigned long lastPirChange = 0;
int lastPirValue = LOW;

// Fade-out tracking
bool fadingOut = false;
unsigned long lastFadeStep = 0;
int fadeBrightness = BRIGHTNESS;

// LED strip
Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// ========================
// HELPERS
// ========================

void clearZone(int start, int end) {
  for (int i = start; i <= end; i++)
    strip.setPixelColor(i, 0);
}

// ----- Zone A: Flashing Blue -----
void blueFlashZoneA() {
  unsigned long now = millis();
  if (now - lastFlashTime > FLASH_SPEED_MS) {
    lastFlashTime = now;
    flashState = !flashState;
  }

  uint32_t color = flashState ? strip.Color(0, 0, 255) : strip.Color(0, 0, 0);

  for (int i = ZONE_A_START; i <= ZONE_A_END; i++) {
    strip.setPixelColor(i, color);
  }
}

// ----- Zone B: Knight Rider -----
void knightRiderZoneB() {
  unsigned long now = millis();
  if (now - lastKRTime < KR_SPEED_MS) return;
  lastKRTime = now;

  clearZone(ZONE_B_START, ZONE_B_END);

  strip.setPixelColor(krPos, strip.Color(255, 0, 0));

  for (int t = 1; t <= KR_TAIL; t++) {
    int left = krPos - t;
    int right = krPos + t;

    int fade = 255 - (255 * t / (KR_TAIL + 1));
    uint32_t tailColor = strip.Color(fade, 0, 0);

    if (left >= ZONE_B_START) strip.setPixelColor(left, tailColor);
    if (right <= ZONE_B_END) strip.setPixelColor(right, tailColor);
  }

  krPos += krDir;
  if (krPos <= ZONE_B_START || krPos >= ZONE_B_END)
    krDir = -krDir;
}

// ----- Non-blocking smooth fade-out -----
void fadeOutStrip() {
  unsigned long now = millis();

  if (!fadingOut) {
    fadingOut = true;
    fadeBrightness = BRIGHTNESS;
    lastFadeStep = now;
  }

  if (now - lastFadeStep > 15) {  // fade speed
    lastFadeStep = now;

    fadeBrightness -= 4;
    if (fadeBrightness < 0) fadeBrightness = 0;

    strip.setBrightness(fadeBrightness);
    strip.show();
  }
}

// Reset fade when motion is detected
void resetFade() {
  fadingOut = false;
  fadeBrightness = BRIGHTNESS;
  strip.setBrightness(BRIGHTNESS);
}

// ========================
// PIR HANDLING
// ========================
void handlePIR() {
  unsigned long now = millis();
  int reading = digitalRead(PIR_PIN);

  if (reading != lastPirValue) {
    lastPirChange = now;
    lastPirValue = reading;
  }

  if (now - lastPirChange > PIR_DEBOUNCE_MS) {
    if (reading == HIGH) {
      motionActive = true;
      lastMotionTime = now;
      resetFade();   // <-- IMPORTANT FIX
    }
  }

  if (motionActive && now - lastMotionTime > ACTIVE_TIME_MS) {
    motionActive = false;
  }
}

// ========================
// SETUP + LOOP
// ========================
void setup() {
  Serial.begin(115200);
  pinMode(PIR_PIN, INPUT);

  strip.begin();
  strip.setBrightness(BRIGHTNESS);
  strip.show();
}

void loop() {
  handlePIR();

  if (motionActive) {
    fadingOut = false;  // Make sure fade stops
    blueFlashZoneA();
    knightRiderZoneB();
  } else {
    fadeOutStrip();
  }

  strip.show();
}
