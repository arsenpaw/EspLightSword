#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "Lightsaber.h"
#include "LedSword.h"

#define LED_PIN   10
#define LED_COUNT 120
#define BCK_PIN 7
#define WS_PIN 6  
#define DIN_PIN 8

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
SwordAudioPlayer audioPlayer(BCK_PIN, WS_PIN, DIN_PIN);
LedSword ledSword(&strip);
Lightsaber lightSword(&ledSword, &audioPlayer);

void setup() {
  USBSerial.begin(115200);
  USBSerial.println("Starting LightSword...");
  
  // Initialize the combined lightsaber service
  if (!lightSword.init()) {
    USBSerial.println("Failed to initialize LightSword service");
    while(1) delay(1000);
  }
  
  // Configure lightsaber
  lightSword.setBrightness(100);
  lightSword.setColor(SwordColor::GREEN);
  lightSword.setAnimationSpeed(10); // Fast ignition
  
  // Set custom sound files (optional)
  lightSword.setSounds("/ignition.mp3", "/hum.mp3", "/clash.mp3", "/extinguish.mp3");
  
  USBSerial.println("LightSword ready!");
}

void loop() {
  static unsigned long lastAction = 0;
  static unsigned long lastClash = 0;

  lightSword.loop();
  if (millis() - lastAction > 15000) {
    if (!lightSword.isOn()) {
      lightSword.ignite();
    } else {
      lightSword.extinguish();
    }
    lastAction = millis();
  }
  
  // Demo: Random clash effects every 15 seconds when sword is on
  // if (lightSword.isOn() && millis() - lastClash > 15000) {
  //   if (random(0, 100) < 30) { // 30% chance
  //     lightSword.clash();
  //     lastClash = millis();
  //   }
  // }
}
