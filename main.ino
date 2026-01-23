
#include <Adafruit_NeoPixel.h>

// ===============================================
// CONFIGURATION
// ===============================================
#define PIR_PIN        27      // PIR OUT pin
#define LED_PIN         5      // WS2812 DATA pin
#define NUM_LEDS       78
#define BRIGHTNESS    150

const unsigned long ON_TIME = 10000UL; // 10 seconds after last motion
const int fadeStep = 5;                // Color fade step per frame
const int delaySpeed = 20;             // Animation speed (ms)

// ===============================================

Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

unsigned long lastMotionTime = 0;
bool active = false;

int fadeValue = 0;
int fadeDir = 1;

void setup() {
  Serial.begin(115200);
  delay(300);

  pinMode(PIR_PIN, INPUT);

  strip.begin();
  strip.setBrightness(BRIGHTNESS);
  strip.show();

  Serial.println("ESP32 PIR + White/Red Fade Ready");
}

void loop() {
  int motion = digitalRead(PIR_PIN);

  if (motion == HIGH) {
    lastMotionTime = millis();
    if (!active) {
      Serial.println("Motion detected -> Start animation");
      active = true;
    }
  }

  if (active && (millis() - lastMotionTime > ON_TIME)) {
    Serial.println("No motion -> LEDs OFF");
    strip.clear();
    strip.show();
    active = false;
    return;
  }

  if (active) {
    fadeWhiteToRed();
  } else {
    delay(50);
  }
}

void fadeWhiteToRed() {
  uint8_t r = 255;
  uint8_t g = 255 - fadeValue;
  uint8_t b = 255 - fadeValue;

  strip.fill(strip.Color(r, g, b), 0, NUM_LEDS);
  strip.show();
  delay(delaySpeed);

  fadeValue += fadeDir * fadeStep;
  if (fadeValue >= 255) {
    fadeValue = 255;
    fadeDir = -1;
  } else if (fadeValue <= 0) {
    fadeValue = 0;
    fadeDir = 1;
  }
}
