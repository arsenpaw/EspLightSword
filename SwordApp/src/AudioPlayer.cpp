#include "AudioPlayer.h"

SwordAudioPlayer::SwordAudioPlayer(uint8_t bclk, uint8_t lrc, uint8_t dout) {
    bclk_pin = bclk;
    lrc_pin = lrc;
    dout_pin = dout;
    initialized = false;
    
    // Initialize pointers
    source = nullptr;
    i2s = nullptr;
    decoder = nullptr;
    player = nullptr;
}

SwordAudioPlayer::~SwordAudioPlayer() {
    if (player) {
        player->end();
        delete player;
    }
    if (decoder) delete decoder;
    if (i2s) {
        i2s->end();
        delete i2s;
    }
    if (source) delete source;
}

bool SwordAudioPlayer::init() {
    if (!initSPIFFS()) {
        USBSerial.println("SPIFFS init failed");
        return false;
    }
    
    // Create components
    source = new AudioSourceSPIFFS("/", "mp3");
    i2s = new I2SStream();
    decoder = new MP3DecoderHelix();
    player = new AudioPlayer(*source, *i2s, *decoder);
    
    if (!source || !i2s || !decoder || !player) {
        USBSerial.println("Failed to create audio components");
        return false;
    }
    
    // Setup I2S output
    auto cfg = i2s->defaultConfig(TX_MODE);
    cfg.pin_bck = bclk_pin;
    cfg.pin_ws = lrc_pin;
    cfg.pin_data = dout_pin;
    cfg.sample_rate = 44100;
    cfg.bits_per_sample = 16;
    cfg.channels = 2;
    
    if (!i2s->begin(cfg)) {
        USBSerial.println("I2S begin failed");
        return false;
    }
    
    // Setup player
    player->setMetadataCallback(printMetaData);
    if (!player->begin()) {
        USBSerial.println("Player begin failed");
        return false;
    }
    
    initialized = true;
    USBSerial.println("Arduino Audio Tools player initialized");
    return true;
}

bool SwordAudioPlayer::play(const char* filepath) {
    if (!initialized || !player) {
        USBSerial.println("Player not initialized");
        return false;
    }
    
    USBSerial.printf("Playing: %s\n", filepath);
    
    // Set the specific file to play
    source->setPath(filepath);
   
    // Start playback
    player->play();
    
    return true;
}

void SwordAudioPlayer::stop() {
    if (player) {
        player->stop();
    }
}

bool SwordAudioPlayer::isInitialized() {
    return initialized;
}

void SwordAudioPlayer::loop() {
    if (player && initialized) {
        player->copy();
    }
}

bool SwordAudioPlayer::initSPIFFS() {
    return SPIFFS.begin(true);
}

void SwordAudioPlayer::printMetaData(MetaDataType type, const char* str, int len) {
    USBSerial.print("==> ");
    USBSerial.print(toStr(type));
    USBSerial.print(": ");
    USBSerial.println(str);
}