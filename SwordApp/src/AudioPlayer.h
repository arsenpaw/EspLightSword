#pragma once

#include <Arduino.h>
#include <SPIFFS.h>
#include "AudioTools.h"
#include "AudioTools/Disk/AudioSourceSPIFFS.h"
#include "AudioTools/AudioCodecs/CodecMP3Helix.h"

using namespace audio_tools;

class SwordAudioPlayer {
private:
    AudioSourceSPIFFS* source;
    I2SStream* i2s;
    MP3DecoderHelix* decoder;
    AudioPlayer* player;
    uint8_t bclk_pin;
    uint8_t lrc_pin;
    uint8_t dout_pin;
    bool initialized;
    
public:
    SwordAudioPlayer(uint8_t bclk, uint8_t lrc, uint8_t dout);
    ~SwordAudioPlayer();
    
    bool init();
    bool play(const char* filepath);
    void stop();
    bool isInitialized();
    void loop();
    
private:
    bool initSPIFFS();
    static void printMetaData(MetaDataType type, const char* str, int len);
};