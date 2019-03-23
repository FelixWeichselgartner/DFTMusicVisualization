"""
**************************************************************
*  Copyright (c) 2019 Felix Weichselgartner
*  all rights reserved
**************************************************************
*  Date of creating this file: 12.12.2019
*  Last changes: 17.01.2019
*  creator: Felix Weichselgartner
*  purpose: practice for exam in 
            Electrical Metrology and Instrumentation
            and usage for project mp3-music-visualization
**************************************************************
*  Implementation of DFT and FFT
**************************************************************
"""

from math import pi
from cmath import exp
from math import atan as arctan

def arctan2(x, y):
    """implementation of the arctan2
    
    Arguments:
        x {double} -- x value
        y {double} -- y value
    
    Returns:
        double -- angle in radian
    """

    #values = ]-pi;pi]
    if x>0:
        return arctan(y/x)
    if x<0:
        if y>0:
            return arctan(y/x)+pi
        if y==0:
            return pi
        if y<0:
            return arctan(y/x)-pi
    if x==0:
        if y==0:
            return 0 #or None actually it has no phase here but 0 is fine
        if y>0:
            return pi/2
        if y<0:
            return -pi/2

def angle(X):
    """calculates the angle of a complex number
    
    Arguments:
        X {complex number} -- the complex number to calculate the angle for
    
    Returns:
        double -- the angle in radian
    """

    return arctan2(X.real, X.imag)
    
def dft(x):
    """implementation of the discrete fourier transformation
    
    Arguments:
        x {list} -- list of values to fourier transform
    
    Returns:
        [list] -- list of the fourier transformed data
    """

    N = len(x)
    X = [None] * N
    l = 0
    wExponent = 1j*(-2)*pi/(N)
    for l in range(N):
        k = 0
        temp = 0
        for k in range(N):
            temp = x[k]*exp(wExponent*k*l) + temp
        X[l] = temp/N
    return X

def idft(x):
    pass

def fft(x):
    """implementation of the fast fourier transformation.
    
    Arguments:
        x {list} -- list of values to fourier transform
    
    Returns:
        list -- list of the fourier transformed data
    """

    N = len(x)
    max = int(N/2)

    #decimation in time
    xs = [None] * max
    xss = [None] * max

    k = 0
    for k in range(max):
        xs[k] = x[2*k]
        xss[k] = x[2*k + 1]

    #fourier transform those both parts
    XS = dft(xs)
    XSS = dft(xss)

    #add both together to actual fourier transformation
    X = [None] * N
    l = 0
    wExponent = 1j*2*pi/N*(-1)
    for l in range(max):
        X[l] = (XS[l] + exp(wExponent*l)*XSS[l])/2

    l = 0
    for l in range(max):
        X[l + max] = (XS[l] - exp(wExponent*l)*XSS[l])/2
    
    return X