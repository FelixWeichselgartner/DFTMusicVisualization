//standard libs
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <complex.h>
#include <signal.h>
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
#define debug 3

//how much bits the adc got -> the more the better
#define BITSOFADC 10

//the width and height of your matrix setup
#define ARRAYWIDTH 8*3
#define ARRAYHEIGHT 8

//the sampling frequency with which the adc works
#define samplingFrequency 44100

//this is the actual delaytime in the sampling loop
#define deltaT 1/samplingFrequency*1000*1000

//the spi session for the adc
static int spi;

//the length of the signal (and therefore fourier) array;
const int length = pow(2, 10); //pow(2, n)
//the 2 channels of the music signal on the adc pins
int channel1 = 0, channel2 = 1;
//spi channel and channel config
int spiChannel = 0, channelConfig = 8;
//pointer for signal and dft to allocate memory
short *signalm;
float complex *fourier;
//contains the calculated information to display on the matrix
int display[ARRAYWIDTH];
int norm = pow(2, 10-4); //pow(2, 4) = 8
//running if 1 - stopping if 0
int running = true;

//handlers for exiting program by pressing ctrl and c
static void ctrl_c_handler(int signum) {
    (void)(signum);
    running = 0;
}

static void setup_handlers() {
    struct sigaction sa = {
        .sa_handler = ctrl_c_handler,
    };

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
}

void saveSignal() {
    FILE *fp;
    fp = fopen("signal.csv", "w");
    for (int i = 0; i < length; i++) {
	fprintf(fp, "%i\n", signalm[i]);
    }
    fclose(fp);
}

void saveFourierTransformation() {
    FILE *fp;
    fp = fopen("fourier.csv", "w");
    for (int i = 0; i < length; i++) {
	fprintf(fp, "%f\n", fourier[i]);
    }
    fclose(fp);
}

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
    printf("retval = %i\n", retval);
    //return (int)retval;
}

/**
 * @brief  sets up the signalm memory, initialises wiringpi, setup for matrix and spi for adc
 * @note   
 * @retval None
 */
void setup() {
    printf("starting:\tallocating signalm memory\n");

    signalm = malloc(length * sizeof(short));

    if (debug == true) {
	printf("success:\tallocated signalm memory\n");
    }

    if (wiringPiSetup() == -1) {
        printf("wiringPi Setup failed!\n");
        exit(EXIT_FAILURE);
    } else if (debug == true) {
	    printf("success:\twiringPi Setup was a success\n");
    }

    //pinMode(gruen, OUTPUT);
    //digitalWrite(gruen, LOW);

    setup_handlers();

    spi = spiSetup(spiChannel);

    myMatrixSetup(ARRAYWIDTH, ARRAYHEIGHT);
    if (debug == true) {
	printf("success:\tmy matrix setup\n");
    }
}

/**
 * @brief  the sampling loop
 * @note   fills the signalm array ((earlier allocated) with analog values from the adc
 * @retval None
 */
void sample() {
    for (int i = 0; i < length; i++) {
        //later:
        //transform each channel on its own and display left channel on left side of matrix
        //right channel on right side of the matrix
        signalm[i] = (mcpAnalogRead(spiChannel, channelConfig, channel1) + mcpAnalogRead(spiChannel, channelConfig, channel2))/2;
	//signalm[i] = mcpAnalogRead(spiChannel, channelConfig, 2);
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
    if (debug > 1)
	printf("step = %i", step);
    int start = 0, end = step;
    for (int i = 0; i < ARRAYWIDTH; i++) {
        display[i] = integrate(start, end);
        start += step;
        end += step;
    }
}

int minValue() {
    int retval = 0;
    for(int i = 1; i < ARRAYWIDTH; i++) {
	if (display[i] < retval) {
	    retval = display[i];
	}
    }
    return retval;
}

int maxValue() {
    int retval = 0;
    for(int i = 1; i < ARRAYWIDTH; i++) {
	if (display[i] > retval) {
	    retval = display[i];
	}
    }
    return retval;
}

/**
 * @brief  norms the display array to ARRAYHEIGHT
 * @note   
 * @retval None
 */
void normTo8Bit() {
    int minVal = minValue();
    for (int i = 0; i < ARRAYWIDTH; i++) {
        display[i] = display[i] - minVal;
	printf("%i: %i, ", i, display[i]);
    }
    printf("\n");

    int maxVal = maxValue();
    printf("maxVal = %i", maxVal);
    int devider = maxVal/8;
    for (int i = 0; i < ARRAYWIDTH; i++) {
        display[i] = display[i]/devider;
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
    if (debug > 1)
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
    fourier = fft(signalm, length);
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

    saveSignal();
    saveFourierTransformation();

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
    printf("program started\n");
    setup();
    printf("success:\tsetup\n");

    printf("starting:\tentering loop now\n");
    //while button wasnt pushed
    while(running) {
        loop();

        if (debug > 1)
	    delay(1000);
    }

    //closes matrix, frees signalm memory, closes spi session
    myMatrixEnd();
    
    printf("Matrix closed\n");
    free(signalm);
    printf("signal and fourier memory freed\n");
    close(spi);
    printf("spi closed\n");
    return;
}
