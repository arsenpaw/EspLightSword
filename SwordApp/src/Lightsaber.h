#pragma once

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "AudioPlayer.h"
#include "LedSword.h"
enum class SwordState {
    OFF,
    IGNITING,
    ON,
    EXTINGUISHING
};

enum class SwordColor {
    BLUE,
    GREEN, 
    RED,
    PURPLE,
    YELLOW,
    WHITE
};

class Lightsaber {
private:
    LedSword* ledSword;
    SwordAudioPlayer* audioPlayer;
    
    SwordState currentState;
    SwordColor currentColor;
    uint8_t brightness;
    
    // Audio paths
    const char* ignitionSound;
    const char* humSound;
    const char* clashSound;
    const char* extinguishSound;
    
    // Timing
    unsigned long lastStateChange;
    unsigned long animationSpeed;
    
    // Animation variables
    int animationProgress;
    bool animationDirection;
    
public:
    Lightsaber(LedSword* ledSword, SwordAudioPlayer* audio);
    
    bool init();
    void loop();
    
    // Main actions
    void ignite();
    void extinguish();
    void clash();
    
    // Configuration
    void setColor(SwordColor color);
    void setBrightness(uint8_t brightness);
    void setAnimationSpeed(unsigned long speed);
    void setSounds(const char* ignition, const char* hum, const char* clash, const char* extinguish);
    
    // Status
    SwordState getState() const;
    bool isOn() const;
    bool isAnimating() const;
    
private:
    // Color helpers
    void setLedSwordColor();
    
    // Audio helpers
    void playIgnitionSound();
    void playHumSound();
    void playClashSound();
    void playExtinguishSound();
};