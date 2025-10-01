#include "SwordService.h"

Sword::Sword(Adafruit_NeoPixel* ledStrip) {
    strip = ledStrip;
    isOn = false;
    brightness = 50;
    currentColor = strip->Color(0, 255, 0);
}


void Sword::on() {
    if (!isOn) {
        fadeIn();
    }
}

void Sword::off() {
    if (isOn) {
        fadeOut();
    }
}

void Sword::setBrightness(uint8_t newBrightness) {
    brightness = newBrightness;
    if (isOn) {
        strip->setBrightness(brightness);
        strip->show();
    }
}

void Sword::setColor(uint8_t red, uint8_t green, uint8_t blue) {
    currentColor = strip->Color(red, green, blue);
    if (isOn) {
        for (int i = 0; i < strip->numPixels(); i++) {
            strip->setPixelColor(i, currentColor);
        }
        strip->show();
    }
}

void Sword::setColor(uint32_t color) {
    currentColor = color;
    if (isOn) {
        for (int i = 0; i < strip->numPixels(); i++) {
            strip->setPixelColor(i, currentColor);
        }
        strip->show();
    }
}

bool Sword::getState() {
    return isOn;
}

uint8_t Sword::getBrightness() {
    return brightness;
}

void Sword::fadeIn() {
    if (isOn) return;
    isOn = true;
    uint8_t pixelsNum = strip->numPixels();
    int delay_per_pixel = 10;
    int oneSideLength = pixelsNum / 2;

    for (int i = 0; i < oneSideLength; i++) {
        strip->setPixelColor(i, currentColor);
        strip->setPixelColor(pixelsNum - i, currentColor);
        strip->show();
        delay(delay_per_pixel);
    }
    strip->setPixelColor(oneSideLength, currentColor);
    strip->show();

}

void Sword::fadeOut() {
    if (!isOn) return;
    uint8_t pixelsNum = strip->numPixels();
    int delay_per_pixel = 10;
    
    for (int i = pixelsNum / 2; i >= 0; i--) {
        strip->setPixelColor(i, 0);
        strip->setPixelColor(pixelsNum - i, 0);
        strip->show();
        delay(delay_per_pixel);
    }
    
    isOn = false;
    strip->clear();
    strip->show();
}