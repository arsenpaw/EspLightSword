#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <SPIFFS.h>
#include <I2SOutput.h>
#include "SPIFFS.h"
#include <WAVFileReader.h>

#define WAV_PATH "/spiffs/lightsaber.wav"  
#define LED_PIN   10
#define LED_COUNT 120
#define RGB_BRIGHTNESS 255
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
#define WS_PIN 6
#define BCK_PIN 7
#define DIN_PIN 8
#define I2C_ADDR 0x60


// I2S speaker pin configuration using the defined pins
i2s_pin_config_t i2s_speaker_pins = {
    .bck_io_num = BCK_PIN,     // Bit Clock (BCLK) - GPIO 6
    .ws_io_num = WS_PIN,      // Word Select (LRC) - GPIO 7
    .data_out_num = DIN_PIN,   // Data Out (DIN) - GPIO 10
    .data_in_num = I2S_PIN_NO_CHANGE  // Not used for output
};


void play(Output* output, const char* fname) {
  int16_t* samples = (int16_t*)malloc(sizeof(int16_t) * 1024);
  if (!samples) {
    USBSerial.println("alloc fail");
    return;
  }
  FILE* fp = fopen(fname, "rb");
  if (!fp) {
    USBSerial.println("open fail");
    free(samples);
    return;
  }
  WAVFileReader* reader = new WAVFileReader(fp);
  USBSerial.printf("sr=%d\n", reader->sample_rate());
  output->start(reader->sample_rate());
  while (true) {
    int samples_read = reader->read(samples, 1024);
    if (samples_read == 0) break;
    output->write(samples, samples_read);
  }
  output->stop();
  fclose(fp);
  delete reader;
  free(samples);
  USBSerial.println("done");
}

void setup() {
  USBSerial.begin(115200);
  strip.begin();
  strip.setBrightness(RGB_BRIGHTNESS);
  for (int i = 0; i < LED_COUNT; i++)
  {
  strip.setPixelColor(i, strip.Color(0, 15, 0));
    strip.show();
  }


  if (!SPIFFS.begin(true)) {
    USBSerial.println("spiffs fail");
    while (1) delay(1000);
  }
  if (!SPIFFS.exists(WAV_PATH)) {
    USBSerial.println("wav missing");
  } else {
    USBSerial.println("wav ok");
  }
}

void loop() {
  static Output* output = nullptr;
  if (!output) {
    output = new I2SOutput(I2S_NUM_0, i2s_speaker_pins);
    USBSerial.println("i2s ready");
  }
  play(output, WAV_PATH);

}