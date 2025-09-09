#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN   3
#define LED_COUNT 120

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  USBSerial.begin(115200);
  strip.begin();
  strip.show();
}

void loop() {
  for (int b = 0; b <= 50; b++) {
    strip.setBrightness(b);
    for (int i = 0; i < LED_COUNT; i++) {
      strip.setPixelColor(i, strip.Color(255, 255, 255));
    }
    strip.show();
    USBSerial.printf("Fade in: %d\n", b);
    delay(5);
  }

  for (int b = 50; b >= 0; b--) {
    strip.setBrightness(b);
    for (int i = 0; i < LED_COUNT; i++) {
      strip.setPixelColor(i, strip.Color(255, 255, 255));
    }
    strip.show();
    USBSerial.printf("Fade out: %d\n", b);
    delay(5);
  }
}
