#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "../mylib/mcp3008/mcp3008.h"
#include <wiringPi.h>
#include <wiringPiSPI.h>

#define NUM_CHANNELS 1
#define SAMPLE_RATE 2*48000
#define SecondsOfSampling 20
#define BLOCK_SIZE 2*480000 //512

//compile command for main function:
//gcc analogInputTest.c ../mylib/mcp3008/mcp3008.c -lwiringPi -o analogInputTest

/**
 * @brief                   samples analog values to a csv file
 * @note              
 * @param  spiChannel:      the current spi channel
 * @param  channelConfig:   the current channel config
 * @param  channel:         the current analog channel
 * @retval None
 */
void analogInputTest(int spiChannel, int channelConfig, int channel) {
    int delaytime = 1/SAMPLE_RATE*1000*1000;
    int v;
    time_t start = time(0);

    FILE *fp;
    fp = fopen("sample.csv", "w");
    if (fp == NULL) {
        printf("couldn't create file");
        return;
    }

    do {
        v = mcpAnalogRead(spiChannel, channelConfig, channel);
        fprintf(fp, "%i\n", v);
        delayMicroseconds(delaytime);
    } while(difftime(time(0), start) < SecondsOfSampling);

    fclose(fp);
    return;
}

/**
 * @brief  setup
 * @note   
 * @retval 
 */
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
