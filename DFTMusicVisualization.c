#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include "DiscreteFourierTransformation/dft.h"

#define ARRAYWIDTH 12

const int length = ; //pow(2, n)
int samplingFrequency = 2 * 20000;
int deltaT = 1/samplingFrequency*1000*1000;
int channel1 = 0, channel2 = 1;
int spiChannel = 0, channelConfig = 8;
short *signal;
float complex *fourier;
int display[ARRAYWIDTH];
int norm = pow(2, 10-4); //pow(2, 4) = 8
int norm8bit;

int integrate(int start, int end) {
    int retval = 0;
    for (int i = start, i < end; i++) {
        retval = fourier[i] + retval;
    }
    return retval;
}

void setup() {
    signal = malloc(length * sizeof(short));
    if (wiringPiSetup() == -1) {
		printf("wiringPi Setup failed!\n");
		exit(EXIT_FAILURE);
	}
	
    //pinMode(gruen, OUTPUT);
	//digitalWrite(gruen, LOW);

	*spi = spiSetup(spiChannel);
}

void sample() {
    for (int i = 0; i < length; i++) {
        signal[i] = (analogRead(spiChannel, channelConfig, channel1) + analogRead(spiChannel, channelConfig, channel2))/(2*norm);
        delayMicroseconds(deltaT);
    }
}

void FormToMatrix() {
    int max = length/2;
    int step = norm8bit = max/ARRAYWIDTH;
    int start = 0, end = step;
    for (int i = 0; i < ARRAYWIDTH; i++) {
        display[i] = integrate(start, end);
        start += step;
        end += step;
    }
}

void normTo8Bit() {
    for (int i = 0; i < ARRAYWIDTH; i++) {
        display[i] = display[i]/norm8bit;
    }
}

void ConsoleOutput() {
    printf("Matrix:\n");
    for(int i = 0; i < ARRAYWIDTH; i++) {
        printf("%i ", i);
    }
    printf("\n");
    for(int i = 0; i < ARRAYWIDTH; i++) {
        printf("%i ", display[i]);
    }
    printf("\n");
}

void MatrixOutput() {
    //output for display
}

void loop() {
    sample();
    fourier = fft(signal, length);
    integrate();
    normTo8Bit();
    ConsoleOutput();
    MatrixOutput();
    free(fourier);
}

void main() {
    setup();
    
    //while button wasnt pushed
    loop();

    free(signal); free(X);
    close(*spi);
    return;
}