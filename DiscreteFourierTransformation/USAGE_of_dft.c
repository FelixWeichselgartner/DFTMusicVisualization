#include "dft.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <complex.h>

#define pi 3.14159265358979323846

//gcc .\USAGE_of_dft.c .\dft.c -o .\USAGE_of_dft.ex

double absoluteComplex(double x, double y) {
    return sqrt(x*x + y*y);
}

void ConsoleDArray(double *x, int length) {
    printf("double array: ");
    for (int i = 0; i < length; i++) {
        printf("%lf ", x[i]);
    }
    printf("\n");
}

void ConsoleCArray(double complex *x, int length) {
    printf("double absolute: ");
    for (int i = 0; i < length; i++) {
        printf("%lf, %lf\n", creal(x[i]), cimag(x[i]));
        //printf("%lf ", absoluteComplex(creal(x[i]), cimag(x[i])));
    }
    printf("\n");
}

void error() {
    printf("error!");
}

void main() {
    double * signal;
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

    double complex *XDFT = malloc(length * sizeof(double complex)), *XFFT = malloc(length * sizeof(double complex));
    if (XDFT == NULL || XFFT == NULL) {
        error();
        return;
    }
    dft(signal, length, XDFT);
    printf("DFT: \n");
    ConsoleCArray(XDFT, length);
    fft(signal, length, XFFT);
    printf("FFT: \n");
    ConsoleCArray(XFFT, length);

}