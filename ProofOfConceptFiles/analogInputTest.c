#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../mylib/mcp3008/mcp3008.h"
#include <time.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

#define NUM_CHANNELS 1
#define SAMPLE_RATE 48000
#define SecondsOfSampling 10
#define BLOCK_SIZE 480000 //512

//compile command for main function:
//gcc analogInputTest.c ../mylib/mcp3008/mcp3008.c -lwiringPi -o analogInputTest

void analogInputTest(int spiChannel, int channelConfig, int channel) {
    time_t start = time(0), startComputation;
    int delaytime = 1/SAMPLE_RATE*1000*1000;
    int v, computationtime;

    FILE *fp;
    fp = fopen("sample.csv", "w");
    if (fp == NULL) {
        printf("couldn't create file");
        return;
    }

    do {
        //startComputation = time(0);
        v = mcpAnalogRead(spiChannel, channelConfig, channel);
        fprintf(fp, "%i\n", v);
        //computationtime = difftime(time(0), startComputation);
	//delayMicroseconds(delaytime-computationtime);
	delayMicroseconds(delaytime);
	//printf("%f\n", difftime(time(0), start));
    } while(difftime(time(0), start) < SecondsOfSampling);

    fclose(fp);
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
