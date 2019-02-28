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

//debug modes:
//1: ends after endtime (param in main()) seconds
//2: 1. mode + prints some information in console + 1 second delay in loop
//3: 1. mode + 2. mode + prints display array to console
#define debug 1

//how much bits the adc got -> the more the better
#define BITSOFADC 10

//the width and height of your matrix setup
#define ARRAYWIDTH 8
#define ARRAYHEIGHT 24

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
int display[ARRAYHEIGHT];
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
    printf("starting:\tallocating signal memory\n");

    signal = malloc(length * sizeof(short));

    if (debug == true) {
	printf("success:\tallocated signal memory\n");
    }

    if (wiringPiSetup() == -1) {
        printf("wiringPi Setup failed!\n");
        exit(EXIT_FAILURE);
    } else if (debug == true) {
	printf("success:\twiringPi Setup was a success\n");
    }

    //pinMode(gruen, OUTPUT);
    //digitalWrite(gruen, LOW);

    spi = spiSetup(spiChannel);

    myMatrixSetup(ARRAYHEIGHT, ARRAYWIDTH);
    if (debug == true) {
	printf("success:\tmy matrix setup\n");
    }
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
        signal[i] = (mcpAnalogRead(spiChannel, channelConfig, channel1) + mcpAnalogRead(spiChannel, channelConfig, channel2))/2;
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
    int step = max/ARRAYHEIGHT;
    int start = 0, end = step;
    for (int i = 0; i < ARRAYHEIGHT; i++) {
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
    for (int i = 0; i < ARRAYHEIGHT; i++) {
        display[i] = display[i]*ARRAYWIDTH/pow(2, BITSOFADC);
    }
}

/**
 * @brief  prints the display array to console -> only for debug mode
 * @note
 * @retval None
 */
void ConsoleOutput() {
    printf("Matrix:\n");
    for(int i = 0; i < ARRAYHEIGHT; i++) {
        printf("%2i ", i);
    }
    printf("\n");
    for(int i = 0; i < ARRAYHEIGHT; i++) {
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
    printf("program going to matrix.c file now\n");
    myMatrixOutput(display);
}

/**
 * @brief  the actual loop
 * @note   sampling, then fourier transformation, then forming and norming and output, finally free the fourier memory
 * @retval None
 */
void loop() {
    sample();
    if (debug > 1) {
	    printf("success:\tsample\n");
    }
    fourier = fft(signal, length);
    if (debug > 1) {
	    printf("success:\tfourier\n");
    }
    FormToMatrix();
    if (debug > 1) {
	    printf("success:\tform to matrix\n");
    }
    normTo8Bit();
    if (debug > 1) {
	    printf("success:\tnorm to 8 bit\n");
    }
    if (debug > 2) {
	    ConsoleOutput();
    }
    MatrixOutput();
    if (debug > 1) {
	    printf("success:\tmatrix output\n");
    }
    free(fourier);
    if (debug > 1) {
	    printf("success:\tfreed fourier memory\n");
    }
}

/**
 * @brief  the main function
 * @note   setup, then loop
 * @retval None
 */
void main() {
    time_t start = time(0);
    short endtime = 30;
    printf("program started\n");
    short running = true;
    setup();
    printf("success:\tsetup\n");

    //while button wasnt pushed
    while(running) {
        loop();

        if (debug > 1)
            delay(1);

        if (debug > 0) {
            //interupts the loop after endtime
            if (difftime(time(0), start) > endtime)
                break;
        }
    }

    //closes matrix, frees signal memory, closes spi session
    myMatrixEnd();
    if (debug > 1)
        printf("Matrix closed\n");
    free(signal);
    if (debug > 1)
        printf("signal and fourier memory freed\n");
    close(spi);
    if (debug > 1)
        printf("spi closed\n");
    return;
}
