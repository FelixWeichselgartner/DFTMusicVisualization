#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <complex.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include "mylib/DiscreteFourierTransformation/c/dft.h"
#include "mylib/mcp3008/mcp3008.h"

#define true 1
#define false 0

#define debug true

#define ARRAYWIDTH 12
#define samplingFrequency 44100
#define deltaT 1/samplingFrequency*1000*1000

static int spi;

const int length = pow(2, 10); //pow(2, n)
int channel1 = 0, channel2 = 1;
int spiChannel = 0, channelConfig = 8;
short *signal;
float complex *fourier;
int display[ARRAYWIDTH];
int norm = pow(2, 10-4); //pow(2, 4) = 8

int integrate(int start, int end) {
    float retval = 0;
    for (int i = start; i < end; i++) {
        retval = sqrt(creal(fourier[i])*creal(fourier[i]) + cimag(fourier[i])*cimag(fourier[i])) + retval;
    }
    return (int)retval;
}

void setup() {
    signal = malloc(length * sizeof(short));
    if (wiringPiSetup() == -1) {
	printf("wiringPi Setup failed!\n");
	exit(EXIT_FAILURE);
    }

    //pinMode(gruen, OUTPUT);
    //digitalWrite(gruen, LOW);

    spi = spiSetup(spiChannel);
}

void sample() {
    for (int i = 0; i < length; i++) {
        //later:
        //transform each channel on its own and display left channel on left side of matrix
        //right channel on right side of the matrix
        signal[i] = (mcpAnalogRead(spiChannel, channelConfig, channel1) + mcpAnalogRead(spiChannel, channelConfig, channel2))/(2);
        delayMicroseconds(deltaT);
    }
}

void FormToMatrix() {
    int max = length/2;
    int step = max/ARRAYWIDTH;
    int start = 0, end = step;
    for (int i = 0; i < ARRAYWIDTH; i++) {
        display[i] = integrate(start, end);
        start += step;
        end += step;
    }
}

void normTo8Bit() {
    for (int i = 0; i < ARRAYWIDTH; i++) {
        display[i] = display[i]*8/pow(2, 10);
    }
}

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

void MatrixOutput() {
    //output for display
}

void loop() {
    sample();
    fourier = fft(signal, length);
    FormToMatrix();
    normTo8Bit();
    if (debug) {
	ConsoleOutput();
    }
    MatrixOutput();
    free(fourier);
}

void main() {
    short running = true;
    setup();

    //while button wasnt pushed
    while(running) {
        loop();
	if (debug) {
	    delay(1);
	}
    }

    free(signal); free(fourier);
    close(spi);
    return;
}
