#include <Adafruit_NeoPixel.h>

// Simple standalone strip test (no PIR required)
#define LED_PIN      5
#define NUM_LEDS     60
#define BRIGHTNESS  150

Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);
  delay(200);

  strip.begin();
  strip.setBrightness(BRIGHTNESS);
  strip.show();

  Serial.println("LED strip test ready");
}

void loop() {
  // Solid color checks
  solidColor(strip.Color(255, 0, 0), 1000);   // Red
  solidColor(strip.Color(0, 255, 0), 1000);   // Green
  solidColor(strip.Color(0, 0, 255), 1000);   // Blue
  solidColor(strip.Color(255, 255, 255), 1000); // White

  // Simple moving pattern checks
  theaterChase(strip.Color(0, 0, 255), 12, 80);   // Blue chase
  theaterChase(strip.Color(255, 0, 0), 12, 80);   // Red chase

  // Full-spectrum rainbow cycle
  rainbowCycle(10);
}

void solidColor(uint32_t color, uint16_t holdMs) {
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, color);
  }
  strip.show();
  delay(holdMs);
}

// Classic theater chase (moving dots) pattern
void theaterChase(uint32_t color, uint8_t iterations, uint16_t wait) {
  for (uint8_t j = 0; j < iterations; j++) {
    for (uint8_t q = 0; q < 3; q++) {
      for (int i = 0; i < NUM_LEDS; i += 3) {
        strip.setPixelColor(i + q, color);
      }

      strip.show();
      delay(wait);

      for (int i = 0; i < NUM_LEDS; i += 3) {
        strip.setPixelColor(i + q, 0);
      }
    }
  }
}

// Rainbow across the whole strip. Based on Adafruit NeoPixel example.
void rainbowCycle(uint8_t wait) {
  for (long firstPixelHue = 0; firstPixelHue < 5 * 65536; firstPixelHue += 256) {
    for (int i = 0; i < strip.numPixels(); i++) {
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    strip.show();
    delay(wait);
  }
}
