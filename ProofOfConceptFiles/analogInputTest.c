#include <stdio.h>
#include <unistd.h>

#include "tinywav/tinywav.h"
#include "../mcp3008/mcp3008.h"
#include <time.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

#define NUM_CHANNELS 1
#define SAMPLE_RATE 48000
#define SecondsOfSampling 10
#define BLOCK_SIZE 512

void analogInputTest(int spiChannel, int channelConfig, int channel) {
    time_t start = time(0);
    int delaytime = 1/SAMPLE_RATE;
    TinyWav tw;
    float buffer[BLOCK_SIZE];
    tinywav_open_write(&tw, NUM_CHANNELS, SAMPLE_RATE, TW_FLOAT32, TW_INLINE, "out/sample.wav");

    do {
        for (int k = 0; k < BLOCK_SIZE; k++) {
            buffer[k] = (float)mcpAnalogRead(spiChannel, channelConfig, channel)*5.0/1023;
	    delayMicroseconds(delaytime);
        }
        tinywav_write_f(&tw, buffer, BLOCK_SIZE);
    } while(difftime(time(0), start) < SecondsOfSampling);

    tinywav_close_write(&tw);
    return;
}

int main() {
    static int spi;
    int channel = 0, spiChannel = 0, channelConfig = 8;
    if (wiringPiSetup() == -1) {
        printf("setup failed\n");
        return 0;
    }
    spi = spiSetup(spiChannel);
    analogInputTest(spiChannel, channelConfig, channel);
    close(spi);
    return 0;
}
