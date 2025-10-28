#pragma once

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "AudioPlayer.h"

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

class LightSwordService {
private:
    Adafruit_NeoPixel* strip;
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
    LightSwordService(Adafruit_NeoPixel* ledStrip, SwordAudioPlayer* audio);
    
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
    // LED animations
    void updateIgnitionAnimation();
    void updateExtinguishAnimation();
    void updateIdleHum();
    void setAllPixels(uint32_t color);
    void clearStrip();
    
    // Color helpers
    uint32_t getSwordColor() const;
    uint32_t dimColor(uint32_t color, float factor) const;
    
    // Audio helpers
    void playIgnitionSound();
    void playHumSound();
    void playClashSound();
    void playExtinguishSound();
};