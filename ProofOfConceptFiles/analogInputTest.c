#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <time.h> 
#include <string.h>

#include "../mylib/mcp3008/mcp3008.h"
#include "../mylib/AlsaAPI/AlsaAPI.h"
#include <wiringPi.h>
#include <wiringPiSPI.h>

#define NUM_CHANNELS 1
#define SAMPLE_RATE 44100
#define SAMPLING_TIME 20
#define BLOCK_SIZE SAMPLE_RATE //512

//compile command for main function:
//gcc analogInputTest.c ../mylib/mcp3008/mcp3008.c ../mylib/AlsaAPI/AlsaAPI.c -lwiringPi -lasound -o analogInputTest

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
    } while(difftime(time(0), start) < SAMPLING_TIME);

    fclose(fp);
    return;
}

/**
 * @brief  setup
 * @note   
 * @retval 
 */
int main(int argc, char* argv[]) {
    printf("> You are running: The Analog Input Test!");
    if (argc > 1) {
        if (!strcmp(argv[1], "-usb")) {
	        int delaytime = 1 / SAMPLE_RATE * 1000 * 1000;
    	    time_t start = time(0);

            int deviceNumber = 1, amountChannels = 2, bufferFrames = 32, samplingRate = SAMPLE_RATE, sampleLength;
            short *sample = NULL;

            initAlsa(deviceNumber, amountChannels, bufferFrames, samplingRate);

            FILE *fp;
            fp = fopen("sampleAlsa.csv", "w");

            if (fp == NULL) {
                printf("[Error] Couldn't create csv-file");
                exit(1);
            }

            do {
                sampleLength = readAlsa(&sample);

                if (sample == NULL) {
                    printf("[SampleError] error occured memory related while sampling");
                    exit(1);
                }

                for (int i = 0; i < sampleLength; i++) {
                    fprintf(fp, "%i;\n", sample[i + 2]);
                }

                free(sample);
                sample = NULL;

            } while (difftime(time(0), start) < SAMPLING_TIME);

            fclose(fp);
            closeAlsa();
            exit(1);
        }
    } else if (!strcmp(argv[1], "-s")) {
        int deviceNumber = 1, amountChannels = 2, bufferFrames = 32, samplingRate = SAMPLE_RATE;
        initAlsa(deviceNumber, amountChannels, bufferFrames, samplingRate);
        recordForSeconds(10);
        closeAlsa();
    } else {
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
}
