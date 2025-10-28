#include "LightSwordService.h"

LightSwordService::LightSwordService(Adafruit_NeoPixel* ledStrip, SwordAudioPlayer* audio) {
    strip = ledStrip;
    audioPlayer = audio;
    
    currentState = SwordState::OFF;
    currentColor = SwordColor::BLUE;
    brightness = 100;
    animationSpeed = 15;
    
    // Default sound paths
    ignitionSound = "/ignition.mp3";
    humSound = "/hum.mp3";
    clashSound = "/clash.mp3";
    extinguishSound = "/extinguish.mp3";
    
    lastStateChange = 0;
    animationProgress = 0;
    animationDirection = true;
}

bool LightSwordService::init() {
    if (!strip) {
        USBSerial.println("LED strip not initialized");
        return false;
    }
    
    strip->begin();
    strip->setBrightness(brightness);
    clearStrip();
    
    if (audioPlayer && !audioPlayer->init()) {
        USBSerial.println("Audio player initialization failed");
        return false;
    }
    
    USBSerial.println("LightSword service initialized");
    return true;
}

void LightSwordService::loop() {
    // Handle audio streaming
    if (audioPlayer) {
        audioPlayer->loop();
    }
    
    // Handle LED animations based on current state
    switch (currentState) {
        case SwordState::IGNITING:
            updateIgnitionAnimation();
            break;
            
        case SwordState::ON:
            updateIdleHum();
            break;
            
        case SwordState::EXTINGUISHING:
            updateExtinguishAnimation();
            break;
            
        case SwordState::OFF:
            // Nothing to do when off
            break;
    }
}

void LightSwordService::ignite() {
    if (currentState == SwordState::OFF) {
        USBSerial.println("Igniting lightsaber");
        currentState = SwordState::IGNITING;
        lastStateChange = millis();
        animationProgress = 0;
        
        playIgnitionSound();
        clearStrip();
    }
}

void LightSwordService::extinguish() {
    if (currentState == SwordState::ON) {
        USBSerial.println("Extinguishing lightsaber");
        currentState = SwordState::EXTINGUISHING;
        lastStateChange = millis();
        animationProgress = strip->numPixels();
        
        playExtinguishSound();
        if (audioPlayer) {
            audioPlayer->stop(); // Stop hum sound
        }
    }
}

void LightSwordService::clash() {
    if (currentState == SwordState::ON) {
        USBSerial.println("Lightsaber clash!");
        playClashSound();
        
        // Flash white for clash effect
        uint32_t originalColor = getSwordColor();
        setAllPixels(strip->Color(255, 255, 255));
        strip->show();
        delay(100);
        setAllPixels(originalColor);
        strip->show();
    }
}

void LightSwordService::updateIgnitionAnimation() {
    if (millis() - lastStateChange >= animationSpeed) {
        uint32_t color = getSwordColor();
        uint8_t pixelsNum = strip->numPixels();
        
        // Center-out ignition animation
        int center = pixelsNum / 2;
        
        if (animationProgress <= center) {
            // Light up pixels from center outward
            int leftPixel = center - animationProgress;
            int rightPixel = center + animationProgress;
            
            if (leftPixel >= 0) {
                strip->setPixelColor(leftPixel, color);
            }
            if (rightPixel < pixelsNum && rightPixel != leftPixel) {
                strip->setPixelColor(rightPixel, color);
            }
            
            strip->show();
            animationProgress++;
        } else {
            // Animation complete
            currentState = SwordState::ON;
            setAllPixels(color);
            strip->show();
            playHumSound();
            USBSerial.println("Lightsaber fully ignited");
        }
        
        lastStateChange = millis();
    }
}

void LightSwordService::updateExtinguishAnimation() {
    if (millis() - lastStateChange >= animationSpeed) {
        uint8_t pixelsNum = strip->numPixels();
        int center = pixelsNum / 2;
        
        if (animationProgress > 0) {
            // Turn off pixels from edges toward center
            int leftPixel = center - animationProgress;
            int rightPixel = center + animationProgress;
            
            if (leftPixel >= 0) {
                strip->setPixelColor(leftPixel, 0);
            }
            if (rightPixel < pixelsNum && rightPixel != leftPixel) {
                strip->setPixelColor(rightPixel, 0);
            }
            
            strip->show();
            animationProgress--;
        } else {
            // Animation complete
            currentState = SwordState::OFF;
            clearStrip();
            USBSerial.println("Lightsaber extinguished");
        }
        
        lastStateChange = millis();
    }
}

void LightSwordService::updateIdleHum() {
    // Subtle pulsing effect when idle
    static unsigned long lastPulse = 0;
    static bool pulseDirection = true;
    static uint8_t pulseIntensity = brightness;
    
    if (millis() - lastPulse >= 50) { // Slow pulse
        if (pulseDirection) {
            pulseIntensity++;
            if (pulseIntensity >= brightness + 20) {
                pulseDirection = false;
            }
        } else {
            pulseIntensity--;
            if (pulseIntensity <= brightness - 20) {
                pulseDirection = true;
            }
        }
        
        strip->setBrightness(pulseIntensity);
        setAllPixels(getSwordColor());
        strip->show();
        lastPulse = millis();
    }
}

void LightSwordService::setColor(SwordColor color) {
    currentColor = color;
    if (currentState == SwordState::ON) {
        setAllPixels(getSwordColor());
        strip->show();
    }
}

void LightSwordService::setBrightness(uint8_t newBrightness) {
    brightness = newBrightness;
    strip->setBrightness(brightness);
    if (currentState == SwordState::ON) {
        strip->show();
    }
}

void LightSwordService::setAnimationSpeed(unsigned long speed) {
    animationSpeed = speed;
}

void LightSwordService::setSounds(const char* ignition, const char* hum, const char* clash, const char* extinguish) {
    ignitionSound = ignition;
    humSound = hum;
    clashSound = clash;
    extinguishSound = extinguish;
}

SwordState LightSwordService::getState() const {
    return currentState;
}

bool LightSwordService::isOn() const {
    return currentState == SwordState::ON;
}

bool LightSwordService::isAnimating() const {
    return currentState == SwordState::IGNITING || currentState == SwordState::EXTINGUISHING;
}

void LightSwordService::setAllPixels(uint32_t color) {
    for (int i = 0; i < strip->numPixels(); i++) {
        strip->setPixelColor(i, color);
    }
}

void LightSwordService::clearStrip() {
    strip->clear();
    strip->show();
}

uint32_t LightSwordService::getSwordColor() const {
    switch (currentColor) {
        case SwordColor::BLUE:
            return strip->Color(0, 100, 255);
        case SwordColor::GREEN:
            return strip->Color(0, 255, 50);
        case SwordColor::RED:
            return strip->Color(255, 0, 0);
        case SwordColor::PURPLE:
            return strip->Color(128, 0, 255);
        case SwordColor::YELLOW:
            return strip->Color(255, 255, 0);
        case SwordColor::WHITE:
            return strip->Color(255, 255, 255);
        default:
            return strip->Color(0, 100, 255); // Default blue
    }
}

uint32_t LightSwordService::dimColor(uint32_t color, float factor) const {
    uint8_t r = (color >> 16) & 0xFF;
    uint8_t g = (color >> 8) & 0xFF;
    uint8_t b = color & 0xFF;
    
    return strip->Color(r * factor, g * factor, b * factor);
}

void LightSwordService::playIgnitionSound() {
    if (audioPlayer && audioPlayer->isInitialized()) {
        audioPlayer->play(ignitionSound);
    }
}

void LightSwordService::playHumSound() {
    if (audioPlayer && audioPlayer->isInitialized()) {
        audioPlayer->play(humSound);
    }
}

void LightSwordService::playClashSound() {
    if (audioPlayer && audioPlayer->isInitialized()) {
        audioPlayer->play(clashSound);
    }
}

void LightSwordService::playExtinguishSound() {
    if (audioPlayer && audioPlayer->isInitialized()) {
        audioPlayer->play(extinguishSound);
    }
}