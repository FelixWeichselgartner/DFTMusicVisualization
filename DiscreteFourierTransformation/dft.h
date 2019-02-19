#ifndef DFT_H_
#define DFT_H_

#include <complex.h>

double absoluteComplex(double, double);

void ConsoleDArray(double *, int);

void ConsoleCArray(double complex *, int);

double arctan2(double, double);

double angle(double complex);

double complex * dft(double *, int);

double complex * fft(double *, int, double complex *);

void zeroPadding(double *, int, int);

void zeroPaddingToNextPOW2N(double *, int);

double * hamming(int);

#endif