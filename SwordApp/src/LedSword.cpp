#include "LedSword.h"

LedSword::LedSword(Adafruit_NeoPixel* ledStrip) {
    strip = ledStrip;
    isOn = false;
    brightness = 50;
    currentColor = strip->Color(0, 255, 0);
}


void LedSword::on() {
    if (!isOn) {
        USBSerial.println("Sword igniting");
        fadeIn();
    }
}

void LedSword::off() {
    if (isOn) {
        USBSerial.println("Sword extinguishing");
        fadeOut();
    }
}

void LedSword::setBrightness(uint8_t newBrightness) {
    brightness = newBrightness;
    if (isOn) {
        strip->setBrightness(brightness);
        strip->show();
    }
}

void LedSword::setColor(uint8_t red, uint8_t green, uint8_t blue) {
    currentColor = strip->Color(red, green, blue);
    if (isOn) {
        for (int i = 0; i < strip->numPixels(); i++) {
            strip->setPixelColor(i, currentColor);
        }
        strip->show();
    }
}

void LedSword::setColor(uint32_t color) {
    currentColor = color;
    if (isOn) {
        for (int i = 0; i < strip->numPixels(); i++) {
            strip->setPixelColor(i, currentColor);
        }
        strip->show();
    }
}

bool LedSword::getState() {
    return isOn;
}

uint8_t LedSword::getBrightness() {
    return brightness;
}

void LedSword::fadeIn() {
    if (isOn) return;
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
    isOn = true;
    USBSerial.println("Sword fully ignited");

}

void LedSword::fadeOut() {
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
    USBSerial.println("Sword extinguished");
}