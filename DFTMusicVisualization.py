from __future__ import print_function

import time
import numpy as np

from mylib.DiscreteFourierTransformation.python.dft import *
from mylib.matrix.Matrix import *

import sys
sys.path.append('lib/pyalsaaudio')
import alsaaudio

AMOUNT_OF_MODULES = 4

samplingFrequency = 44100
ARRAY_WIDTH = 8 * AMOUNT_OF_MODULES
ARRAY_HEIGHT = 8
ONEMATRIX_WIDTH = 8
ONEMATRIX_HEIGHT = 8

display = [0] * ARRAY_WIDTH

def integrate(fourier, start, end):
    retval = 0
    for c in range(start, end):
        retval += abs(fourier[c])
    return retval

def createWidth(fourier):
    max = int(len(fourier)*20000/samplingFrequency)
    interval = int(max/ARRAY_WIDTH/2)
    start, end = 0, interval
    for i in range(int(ARRAY_WIDTH / 2)):
        display[i + int(ARRAY_WIDTH/2)] = integrate(fourier, start, end)
        start += interval
        end += interval
    norm()

def maximum(theList):
    retval = 0
    for i in range(10, len(theList)):
        if i > retval:
            retval = i
    return retval

def norm():
    standardValue = 15
    norm = standardValue
    #if maximum(display) > standardValue:
    #    norm = max(display)
    for i in range(ARRAY_WIDTH):
        display[i] = int(display[i] * 8 / norm)

def main():
    matrix = Matrix(8 * 8 * AMOUNT_OF_MODULES, 18, 800000, 10, 32, False, 0)
    matrix.clearMatrix()
    matrix.setBrightness(63)

    device = 'hw:1'
    inp = alsaaudio.PCM(alsaaudio.PCM_CAPTURE,
                        alsaaudio.PCM_NONBLOCK, device=device)

    # Set attributes: Mono, 44100 Hz, 16 bit little endian samples
    inp.setchannels(1)
    inp.setrate(44100)
    inp.setformat(alsaaudio.PCM_FORMAT_S16_LE)
    inp.setperiodsize(160)

    while True:
        #print('infinite loop')
        # Read data from device
        l, data = inp.read()
        signal = []

        if not l:
            continue

        for d in data:
            signal.append(d)

        #print('len of signal: ' + str(len(signal)))
        if len(signal) is not 0:
            fourier = fft(signal)
        createWidth(fourier)

        count = ARRAY_WIDTH - 1
        for i in range(2 * ONEMATRIX_WIDTH):
            display[i] = display[count]
            #print(str(i) + ' ' + str(display[i]) + '   ' + str(count) + ' ' + str(display[count]))
            count -= 1

        if len(signal) is not 0:
            matrix.arrayToMatrix(display, AMOUNT_OF_MODULES, ARRAY_WIDTH, ARRAY_HEIGHT, ONEMATRIX_WIDTH)
        time.sleep(5/1000)

if __name__ == "__main__":
    main()

