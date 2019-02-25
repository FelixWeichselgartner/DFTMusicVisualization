#include "tinywav/tinywav.h"
#include "/../mcp3008/mcp3008.h"
#include <time.h>

#define NUM_CHANNELS 1
#define SAMPLE_RATE 48000
#define SecondsOfSampling 10
#define BLOCK_SIZE 512

void analogInputTest(int spiChannel, int channelConfig, int channel) {
    time_t starttime = time(0);
    tinywav tw;
    float buffer[BLOCK_SIZE];
    tinywav_open_write(&tw,
                       NUM_CHANNELS,
                       SAMPLE_RATE,
                       TW_FLOAT32,          
                       TW_INLINE,                   
                       "out/sample.wav"
    );

    do {
        for (int k = 0; k < BLOCK_SIZE; k++) {
            buffer[k] = mcpAnalogRead(spiChannel, channelConfig, channel);
        }
        tinywav_write_f(&tw, buffer, BLOCK_SIZE);
    } while(difftime(time(0), start) < SecondsOfSampling);

    tinywav_close(&tw);
    return;
}