#include "dft.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <complex.h>

#define pi 3.14159265358979323846

//gcc .\USAGE_of_dft.c .\dft.c -o .\USAGE_of_dft.ex

void error() {
    printf("error!");
}

void main() {
    double *signal;
    int length = 8;
    double inkrement = 2*pi/length;
    double n = 0;
    signal = malloc(length * sizeof(double));
    if (signal == NULL) {
        error();
        return;
    }
    printf("original signal: ");
    for (int i = 0; i < length; i++) {
        signal[i] = sin(n)*5;
        n += inkrement;
    }
    ConsoleDArray(signal, length);

    double complex *XDFT, *XFFT;
    if (XDFT == NULL || XFFT == NULL) {
        error();
        return;
    }
    XDFT = dft(signal, length);
    printf("DFT: \n");
    ConsoleCArray(XDFT, length);
    XFFT = fft(signal, length, XFFT);
    printf("FFT: \n");
    ConsoleCArray(XFFT, length);
    free(signal); free(XDFT); free(XFFT);
}