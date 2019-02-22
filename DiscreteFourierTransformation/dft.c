#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>

#define pi 3.14159265358979323846

float absoluteComplex(float x, float y) {
    return sqrt(x * x + y * y);
}

void ConsoleDArray(float *x, int length) {
    printf("float array: ");
    for (int i = 0; i < length; i++)
    {
        printf("%lf ", x[i]);
    }
    printf("\n\n");
}

void ConsoleCArray(float complex *x, int length) {
    printf("float absolute: \n");
    for (int i = 0; i < length; i++)
    {
        //printf("%i: %lf, %lf;\n", i, creal(x[i]), cimag(x[i]));
        printf("%lf ", absoluteComplex(creal(x[i]), cimag(x[i])));
    }
    printf("\n\n");
}

float arctan2(float x, float y) {
    //values = ]-pi;pi]
    if (x>0) {
        return atan(y/x);
    } else if (x<0) {
        if (y>0) {
            return atan(y/x)+pi;
        } else if (y==0) {
            return pi;
        } else if (y<0) {
            return atan(y/x)-pi;
        }
    } else if (x==0) {
        if (y==0) {
            return 0; //or None actually it has no phase here but 0 is fine
        } else if (y>0) {
            return pi/2;
        } else if (y<0) {
            return -pi/2;
        }
    }
}

float angle(float complex X){
    return arctan2(creal(X), cimag(X));
}

float complex * dft(short *x, int N) {
    float complex *X;
    X = malloc(N * sizeof(float complex));
    float complex wExponent = 1*I*(-2)*pi/(N);
    float complex temp;
    for (int l = 0; l < N; l++) {
        temp = 0 + 0 * I;
        for (int k = 0; k < N; k++) {
            temp = x[k]*cexp(wExponent*k*l) + temp;
        }
        X[l] = temp/N;
    }

    return X;
}

float complex * fft(short *x, int N) {
    int max = N/2;

    //decimation in time
    short *xs = malloc(max * sizeof(short));
    short *xss = malloc(max * sizeof(short));

    for (int k = 0; k < max; k++) {
        xs[k] = x[2*k];
        xss[k] = x[2*k + 1];
    }

    //fourier transform those both parts
    float complex *XS, *XSS;
    XS = dft(xs, max);
    XSS = dft(xss, max);

    //add both together to actual fourier transformation
    float complex *X = malloc(N * sizeof(float complex));
    complex float wExponent = 1*I*2*pi/N*(-1);

    for (int l = 0; l < max; l++) {
        X[l] = (XS[l] + cexp(wExponent*l)*XSS[l])/2;
    }

    for (int l = 0; l < max; l++) {
        X[l + max] = (XS[l] - cexp(wExponent*l)*XSS[l])/2;
    }

    free(xs); free(xss); free(XS); free(XSS);
    return X;
}

void zeroPadding(float *function, int N, int n) {
    function = (float *)realloc(function, n * sizeof(float));
    for (int x = n - N; x < n; x++) {
        function[x] = 0; 
    }
}

void zeroPaddingToNextPOW2N(float *function, int N) {
    int n = 1;
    while (N > pow(2, n)) {
        n++;
    }
    int add = pow(2, n) - N;
    zeroPadding(function, add, N);
}

float * vonHann(float alpha, float beta, int N) {
    float *vonHann = malloc(N * sizeof(float));
    for (int n = 0; n < N; n++) {
        vonHann[n] = alpha - beta * cos(2 * pi * n / (N - 1));
    }
    return vonHann;
}

float * hamming(int N) {
    float alpha = 25.0/46;
    float beta = 1 - alpha;
    return vonHann(alpha, beta , N);
}
