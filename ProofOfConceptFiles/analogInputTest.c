#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "tinywav/tinywav.h"
#include "../mcp3008/mcp3008.h"
#include <time.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

#define NUM_CHANNELS 1
#define SAMPLE_RATE 48000
#define SecondsOfSampling 10
#define BLOCK_SIZE 480000 //512

#define INCLUDE_CSV_FILE
#define INCLUDE_MAIN

//compile command for main function:
//gcc analogInputTest.c ../mcp3008/mcp3008.c tinywav/tinywav.c -lwiringPi -o analogInputTest

void analogInputTest(int spiChannel, int channelConfig, int channel) {
    time_t start = time(0);
    int delaytime = 1/SAMPLE_RATE;
    TinyWav tw;
    float buffer[BLOCK_SIZE];
    //assertion error if you take a path with a not existing folder:
    //e.g.: out/sample.wav
    tinywav_open_write(&tw, NUM_CHANNELS, SAMPLE_RATE, TW_FLOAT32, TW_INLINE, "sample.wav");

    #ifdef INCLUDE_CSV_FILE
    FILE *fp;
    fp = fopen("sample.csv", "w");
    if (fp == NULL) {
	printf("couldn't create file");
	return;
    }
    #endif

    do {
        for (int k = 0; k < BLOCK_SIZE; k++) {
            buffer[k] = (float)mcpAnalogRead(spiChannel, channelConfig, channel);
	    delayMicroseconds(delaytime);
	    #ifdef INCLUDE_CSV_FILE
	    fprintf(fp, "%f\n", buffer[k]);
	    #endif
        }
        tinywav_write_f(&tw, buffer, BLOCK_SIZE);
    } while(difftime(time(0), start) < SecondsOfSampling);

    #ifdef INCLUDE_CSV_FILE
    fclose(fp);
    #endif
    tinywav_close_write(&tw);
    return;
}

#ifdef INCLUDE_MAIN
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
#endif
