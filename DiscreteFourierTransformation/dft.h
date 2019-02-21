#ifndef DFT_H_
#define DFT_H_

#include <complex.h>

float absoluteComplex(float, float);

void ConsoleDArray(float *, int);

void ConsoleCArray(float complex *, int);

float arctan2(float, float);

float angle(float complex);

float complex * dft(float *, int);

float complex * fft(float *, int, float complex *);

void zeroPadding(float *, int, int);

void zeroPaddingToNextPOW2N(float *, int);

float * hamming(int);

#endif