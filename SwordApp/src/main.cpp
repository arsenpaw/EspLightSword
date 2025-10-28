#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "SwordService.h"
#include "AudioPlayer.h"

#define LED_PIN   10
#define LED_COUNT 120
#define AUDIO_PATH "/lightsaber.mp3"
#define BCK_PIN 6
#define WS_PIN 7  
#define DIN_PIN 8

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
Sword sword(&strip);
SwordAudioPlayer audioPlayer(BCK_PIN, WS_PIN, DIN_PIN);

void setup() {
  Serial.begin(115200);
  strip.begin();
  strip.show();
  
  sword.setBrightness(50);
  sword.setColor(0, 255, 0); // Green lightsaber
  
  // Initialize audio player
  if (!audioPlayer.init()) {
    Serial.println("Failed to initialize audio player");
  } else {
    Serial.println("Audio player ready");
  }
}

void loop() {
  static unsigned long lastAction = 0;
  static bool swordState = false;
  
  // Handle audio streaming
  audioPlayer.loop();
  
  // Toggle sword every 5 seconds
  if (millis() - lastAction > 5000) {
    if (!swordState) {
      Serial.println("Lightsaber ON");
      sword.on();
      
      // Play lightsaber ignition sound after LED animation starts
      if (audioPlayer.isInitialized()) {
        audioPlayer.play(AUDIO_PATH);
      }
      swordState = true;
    } else {
      Serial.println("Lightsaber OFF");
      audioPlayer.stop();
      sword.off();
      swordState = false;
    }
    lastAction = millis();
  }
}
