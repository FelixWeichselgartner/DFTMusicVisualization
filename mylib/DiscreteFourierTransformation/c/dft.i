/* File: dft.i */
%module dft

%{
    #define SWIG_FILE_WITH_INIT
    #include "dft.h"
%}

float absoluteComplex(float, float);
float arctan2(float, float);
float angle(float complex);
float complex * dft(int *, int);
float complex * fft(int *, int);
void zeroPadding(float *, int, int);
void zeroPaddingToNextPOW2N(float *, int);
float * vonHann(float, float, int);
float * hamming(int);