
#include <Adafruit_NeoPixel.h>

// ===============================================
// CONFIGURATION
// ===============================================
#define PIR_PIN        27      // PIR OUT pin
#define LED_PIN         5      // WS2812 DATA pin
#define NUM_LEDS       60
#define BRIGHTNESS    150

const unsigned long ON_TIME = 10000UL; // 10 seconds after last motion
const int tailFade = 40;               // Higher = longer tails
const int delaySpeed = 20;             // Animation speed (ms)

// ===============================================

Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// Scanner state
int pos = 0;
int dir = 1;

unsigned long lastMotionTime = 0;
bool active = false;

// Color cycle index
int colorIndex = 0;

// Color palette (orange, white, red, blue)
uint32_t colors[] = {
  Adafruit_NeoPixel::Color(255, 80, 0),
  Adafruit_NeoPixel::Color(255, 255, 200),
  Adafruit_NeoPixel::Color(255, 0, 0),
  Adafruit_NeoPixel::Color(0, 0, 255)
};
const int numColors = 4;

void setup() {
  Serial.begin(115200);
  delay(300);

  pinMode(PIR_PIN, INPUT);

  strip.begin();
  strip.setBrightness(BRIGHTNESS);
  strip.show();

  Serial.println("ESP32 PIR + Fading Knight Rider Ready");
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
    fadingKnightRider();
  } else {
    delay(50);
  }
}

void fadingKnightRider() {
  for (int i = 0; i < NUM_LEDS; i++) {
    uint32_t c = strip.getPixelColor(i);

    uint8_t r = (c >> 16) & 0xFF;
    uint8_t g = (c >>  8) & 0xFF;
    uint8_t b =  c        & 0xFF;

    r = (r > tailFade) ? r - tailFade : 0;
    g = (g > tailFade) ? g - tailFade : 0;
    b = (b > tailFade) ? b - tailFade : 0;

    strip.setPixelColor(i, r, g, b);
  }

  strip.setPixelColor(pos, colors[colorIndex]);

  strip.show();
  delay(delaySpeed);

  pos += dir;

  if (pos <= 0 || pos >= NUM_LEDS - 1) {
    dir = -dir;
    colorIndex = (colorIndex + 1) % numColors;
  }
}
