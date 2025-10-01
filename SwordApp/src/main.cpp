#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "SwordService.h"

#define LED_PIN   10
#define LED_COUNT 120

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
Sword sword(&strip);

void setup() {
  USBSerial.begin(115200);
  strip.begin();
  strip.show();
  
  sword.setBrightness(50);
}

void loop() {
  USBSerial.println("Sword ON");
  sword.on();
  
  delay(2000);
  
  // USBSerial.println("Sword OFF");
  // sword.off();
  
  delay(2000);
}
