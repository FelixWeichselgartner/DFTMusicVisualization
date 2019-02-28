//standard libs
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <complex.h>
//wiringPi
#include <wiringPi.h>
#include <wiringPiSPI.h>
//my own lib
#include "mylib/DiscreteFourierTransformation/c/dft.h"
#include "mylib/mcp3008/mcp3008.h"
#include "mylib/matrix/matrix.h"

#define true 1
#define false 0

//debug mode
#define debug 2

//how much bits the adc got -> the more the better
#define BITSOFADC 10

//the width and height of your matrix setup
#define ARRAYWIDTH 8
#define ARRAYHEIGHT 8

//the sampling frequency with which the adc works
#define samplingFrequency 44100

//this is the actual delaytime in the sampling loop
#define deltaT 1/samplingFrequency*1000*1000

//the spi session for the adc
static int spi;

//the length of the signal (and therefore fourier) array;
const int length = pow(2, 9); //pow(2, n)
//the 2 channels of the music signal on the adc pins
int channel1 = 0, channel2 = 1;
//spi channel and channel config
int spiChannel = 0, channelConfig = 8;
//pointer for signal and dft to allocate memory
short *signal;
float complex *fourier;
//contains the calculated information to display on the matrix
int display[ARRAYWIDTH];
int norm = pow(2, 10-4); //pow(2, 4) = 8

/**
 * @brief           integrates a part of the fourier array with rectangular integration
 * @note   
 * @param  start:   the index of the starting point
 * @param  end:     the index of the ending point
 * @retval          the actual integration value (sum)
 */
int integrate(int start, int end) {
    float retval = 0;
    for (int i = start; i < end; i++) {
        retval = sqrt(creal(fourier[i])*creal(fourier[i]) + cimag(fourier[i])*cimag(fourier[i])) + retval;
    }
    return (int)retval;
}

/**
 * @brief  sets up the signal memory, initialises wiringpi, setup for matrix and spi for adc
 * @note   
 * @retval None
 */
void setup() {
    signal = malloc(length * sizeof(short));
    if (wiringPiSetup() == -1) {
        printf("wiringPi Setup failed!\n");
        exit(EXIT_FAILURE);
    }

    //pinMode(gruen, OUTPUT);
    //digitalWrite(gruen, LOW);

    spi = spiSetup(spiChannel);

    myMatrixSetup();
}

/**
 * @brief  the sampling loop
 * @note   fills the signal array ((earlier allocated) with analog values from the adc
 * @retval None
 */
void sample() {
    for (int i = 0; i < length; i++) {
        //later:
        //transform each channel on its own and display left channel on left side of matrix
        //right channel on right side of the matrix
        signal[i] = (mcpAnalogRead(spiChannel, channelConfig, channel1) + mcpAnalogRead(spiChannel, channelConfig, channel2))/(2);
        delayMicroseconds(deltaT);
    }
}

/**
 * @brief  integrates the fourier array with integration over ARRAYWIDTH parts of the hearable part and saves that in display array
 * @note   20.000Hz is the maximum frequency humans can hear
 * @retval None
 */
void FormToMatrix() {
    int max = length*20000/samplingFrequency;
    int step = max/ARRAYWIDTH;
    int start = 0, end = step;
    for (int i = 0; i < ARRAYWIDTH; i++) {
        display[i] = integrate(start, end);
        start += step;
        end += step;
    }
}

/**
 * @brief  norms the display array to ARRAYHEIGHT
 * @note   
 * @retval None
 */
void normTo8Bit() {
    for (int i = 0; i < ARRAYWIDTH; i++) {
        display[i] = display[i]*ARRAYHEIGHT/pow(2, BITSOFADC);
    }
}

/**
 * @brief  prints the display array to console -> only for debug mode
 * @note   
 * @retval None
 */
void ConsoleOutput() {
    printf("Matrix:\n");
    for(int i = 0; i < ARRAYWIDTH; i++) {
        printf("%2i ", i);
    }
    printf("\n");
    for(int i = 0; i < ARRAYWIDTH; i++) {
        printf("%2i ", display[i]);
    }
    printf("\n");
}

/**
 * @brief  uses the matrix.h from mylib to print the display array to matrix
 * @note   
 * @retval None
 */
void MatrixOutput() {
    myMatrixOutput(display);
}

/**
 * @brief  the actual loop
 * @note   sampling, then fourier transformation, then forming and norming and output, finally free the fourier memory
 * @retval None
 */
void loop() {
    sample();
    fourier = fft(signal, length);
    FormToMatrix();
    normTo8Bit();
    if (debug == true) {
	    ConsoleOutput();
    }
    MatrixOutput();
    free(fourier);
}

/**
 * @brief  the main function
 * @note   setup, then loop
 * @retval None
 */
void main() {
    short running = true;
    short endSoon = 0;
    setup();

    //while button wasnt pushed
    while(running) {
        loop();
        if (debug) {
            delay(1);
        }

        //interupts the loop
        if (debug == 2) {
            endSoon++;
            if (endSoon > 100) {
                break;
            }
        }
    }

    //closes matrix, frees signal memory, closes spi session
    myMatrixEnd();
    printf("Matrix closed\n");
    free(signal);
    printf("signal and fourier memory freed\n");
    close(spi);
    printf("spi closed\n");
    return;
}
