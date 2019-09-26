#include "dft.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <complex.h>

#define pi 3.14159265358979323846

//gcc .\USAGE_of_dft.c .\dft.c -o .\USAGE_of_dft.exe

void error() {
    printf("error!");
}

void main() {
    float *signal;
    int length = 8;
    float inkrement = 2*pi/length;
    float n = 0;
    signal = malloc(length * sizeof(float));

    if (signal == NULL) {
        error();
        return;
    }

    printf("original signal: \n");
    for (int i = 0; i < length; i++) {
        signal[i] = sin(n)*5;
        n += inkrement;
    }
    ConsoleDArray(signal, length);

    float complex *XDFT, *XFFT;

    XDFT = dft(signal, length);
    printf("DFT: \n");
    ConsoleCArray(XDFT, length);
    XFFT = fft(signal, length);
    printf("FFT: \n");
    ConsoleCArray(XFFT, length);
    free(signal); free(XDFT); free(XFFT);
}