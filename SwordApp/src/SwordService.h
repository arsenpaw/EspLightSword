#pragma once

#include <Adafruit_NeoPixel.h>

class Sword {
private:
    Adafruit_NeoPixel* strip;
    bool isOn;
    uint8_t brightness;
    uint32_t currentColor;
    
    void fadeIn();
    void fadeOut();
    
public:
    Sword(Adafruit_NeoPixel* ledStrip);
    
    void on();
    void off();
    void setBrightness(uint8_t brightness);
    void setColor(uint8_t red, uint8_t green, uint8_t blue);
    void setColor(uint32_t color);
    
    bool getState();
    uint8_t getBrightness();
};