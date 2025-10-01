#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <SPIFFS.h>
#include <I2SOutput.h>
#include "SPIFFS.h"
#include <WAVFileReader.h>
#define WAV_PATH "/spiffs/test.wav"  
static const char* TAG = "ESP_LIGHTSWORD";

#define LED_PIN   10
#define LED_COUNT 120
#define RGB_BRIGHTNESS 255
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

#define SDA_PIN 6
#define SCL_PIN 7
#define I2C_ADDR 0x60


// I2S speaker pin configuration using the defined pins
i2s_pin_config_t i2s_speaker_pins = {
    .bck_io_num = SDA_PIN,     // Bit Clock (BCLK) - GPIO 6
    .ws_io_num = SCL_PIN,      // Word Select (LRC) - GPIO 7
    .data_out_num = LED_PIN,   // Data Out (DIN) - GPIO 10
    .data_in_num = I2S_PIN_NO_CHANGE  // Not used for output
};

void play(Output *output, const char *fname)
{
  int16_t *samples = (int16_t *)malloc(sizeof(int16_t) * 1024);
  // open the file on the sdcard
  FILE *fp = fopen(fname, "rb");
  // create a new wave file writer
  WAVFileReader *reader = new WAVFileReader(fp);
  ESP_LOGI(TAG, "Start playing");
  output->start(reader->sample_rate());
  ESP_LOGI(TAG, "Opened wav file");
  // read until theres no more samples
  while (true)
  {
    int samples_read = reader->read(samples, 1024);
    if (samples_read == 0)
    {
      break;
    }
    ESP_LOGI(TAG, "Read %d samples", samples_read);
    output->write(samples, samples_read);
    ESP_LOGI(TAG, "Played samples");
  }
  // stop the input
  output->stop();
  fclose(fp);
  delete reader;
  free(samples);
  ESP_LOGI(TAG, "Finished playing");
}

void setup() {
  USBSerial.begin(115200);
  strip.begin();
  strip.show();

}

void loop() {
  strip.setBrightness(RGB_BRIGHTNESS);
  for (int i = 0; i < LED_COUNT; i++) {
    strip.setPixelColor(i, strip.Color(0, 15, 0));
  }
  strip.show();
  USBSerial.printf("Fade in: %d\n", RGB_BRIGHTNESS);
  Output *output = new I2SOutput(I2S_NUM_0, i2s_speaker_pins);
  while (true)
  {
    play(output, WAV_PATH);
    delay(1000);
  }
}
