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
#define SecondsOfSampling 20
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
    } while(difftime(time(0), start) < SecondsOfSampling);

    fclose(fp);
    return;
}

/**
 * @brief  setup
 * @note   
 * @retval 
 */
int main(int argc, char* argv[]) {
    if (argc > 1) {
        if (!strcmp(argv[1], "-usb")) {
            deviceNumber = 1;
            amountChannels = 1;
            bufferFrames = 128;
            samplingRate = SAMPLE_RATE;
            short *sample;
            int sampleLength;
            init(int deviceNumber, int amountChannels, int bufferFrames, int samplingRate);
            FILE *fp;
            fp = fopen("sample.csv", "w");
            if (fp == NULL) {
                printf("[Error] Couldn't create csv-file");
                exit(1);
            }

            do {
                read(sample, &sampleLength);
                for (int i = 0; i < length; i++) {
                    fprintf(fp, "%i;\n", sample[i]);
                    free(sample);
                }
            } while (difftime(time(0), start) < SecondsOfSampling);
            
            fclose(fp);
            close();
            exit(1);
        }
    } else {
        static int spi;
        int channel = 0, spiChannel = 0, channelConfig = 8;
        if (wiringPiSetup() == -1)
        {
            printf("setup failed\n");
            return 0;
        }
        spi = spiSetup(spiChannel);
        analogInputTest(spiChannel, channelConfig, channel);
        close(spi);
        return 0;
    }    
}
