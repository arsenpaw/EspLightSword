#include "Lightsaber.h"

Lightsaber::Lightsaber(LedSword* ledSword, SwordAudioPlayer* audio) {
    this->ledSword = ledSword;
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

bool Lightsaber::init() {
    if (!ledSword) {
        USBSerial.println("LedSword not initialized");
        return false;
    }
    
    // Set initial configuration
    ledSword->setBrightness(brightness);
    setLedSwordColor();
    
    if (audioPlayer && !audioPlayer->init()) {
        USBSerial.println("Audio player initialization failed");
        return false;
    }
    
    USBSerial.println("Lightsaber service initialized");
    return true;
}

void Lightsaber::loop() {
    // Handle audio streaming
    if (audioPlayer) {
        audioPlayer->loop();
    }
    
    // Simple state management - LedSword handles all animations
    switch (currentState) {
        case SwordState::IGNITING:
            // Check if ignition animation should be complete
            if (millis() - lastStateChange > 2000) { // 2 second ignition
                currentState = SwordState::ON;
                playHumSound();
                USBSerial.println("Lightsaber fully ignited");
            }
            break;
            
        case SwordState::EXTINGUISHING:
            // Check if extinguish animation should be complete
            if (millis() - lastStateChange > 2000) { // 2 second extinguish
                currentState = SwordState::OFF;
                USBSerial.println("Lightsaber extinguished");
            }
            break;
            
        case SwordState::ON:
        case SwordState::OFF:
            // Nothing to do - LedSword handles all visual effects
            break;
    }
}

void Lightsaber::ignite() {
    if (currentState == SwordState::OFF) {
        USBSerial.println("Igniting lightsaber");
        currentState = SwordState::IGNITING;
        lastStateChange = millis();
        
        playIgnitionSound();
        setLedSwordColor();
        ledSword->on();
    }
}

void Lightsaber::extinguish() {
    if (currentState == SwordState::ON) {
        USBSerial.println("Extinguishing lightsaber");
        currentState = SwordState::EXTINGUISHING;
        lastStateChange = millis();

        playExtinguishSound();
        if (audioPlayer) {
            audioPlayer->stop(); // Stop hum sound
        }
        
        // Use LedSword's built-in extinguish animation
        ledSword->off();
    }
}

void Lightsaber::clash() {
    if (currentState == SwordState::ON) {
        USBSerial.println("Lightsaber clash!");
        playClashSound();
        
        // Simple clash effect - just play sound for now
        // Complex visual effects would require LedSword to support them
    }
}







void Lightsaber::setColor(SwordColor color) {
    currentColor = color;
    if (currentState == SwordState::ON) {
        setLedSwordColor();
    }
}

void Lightsaber::setBrightness(uint8_t newBrightness) {
    brightness = newBrightness;
    ledSword->setBrightness(brightness);
}

void Lightsaber::setAnimationSpeed(unsigned long speed) {
    animationSpeed = speed;
}

void Lightsaber::setSounds(const char* ignition, const char* hum, const char* clash, const char* extinguish) {
    ignitionSound = ignition;
    humSound = hum;
    clashSound = clash;
    extinguishSound = extinguish;
}

SwordState Lightsaber::getState() const {
    return currentState;
}

bool Lightsaber::isOn() const {
    return currentState == SwordState::ON;
}

bool Lightsaber::isAnimating() const {
    return currentState == SwordState::IGNITING || currentState == SwordState::EXTINGUISHING;
}

void Lightsaber::setLedSwordColor() {
    switch (currentColor) {
        case SwordColor::BLUE:
            ledSword->setColor(0, 100, 255);
            break;
        case SwordColor::GREEN:
            ledSword->setColor(0, 255, 50);
            break;
        case SwordColor::RED:
            ledSword->setColor(255, 0, 0);
            break;
        case SwordColor::PURPLE:
            ledSword->setColor(128, 0, 255); 
            break;
        case SwordColor::YELLOW:
            ledSword->setColor(255, 255, 0);
            break;
        case SwordColor::WHITE:
            ledSword->setColor(255, 255, 255);
            break;
        default:
            ledSword->setColor(0, 100, 255); // Default blue
            break;
    }
}

void Lightsaber::playIgnitionSound() {
    if (audioPlayer && audioPlayer->isInitialized()) {
        audioPlayer->play(ignitionSound);
    }
}

void Lightsaber::playHumSound() {
    if (audioPlayer && audioPlayer->isInitialized()) {
        audioPlayer->play(humSound);
    }
}

void Lightsaber::playClashSound() {
    if (audioPlayer && audioPlayer->isInitialized()) {
        audioPlayer->play(clashSound);
    }
}

void Lightsaber::playExtinguishSound() {
    if (audioPlayer && audioPlayer->isInitialized()) {
        audioPlayer->play(extinguishSound);
    }
}