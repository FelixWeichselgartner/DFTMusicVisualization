import time
import numpy as np

import alsaaudio
from mylib.DiscreteFourierTransformation.python.dft import *    
from mylib.matrix.Matrix import *

samplingFrequency = 44100
ARRAY_WIDTH = 8 * 4

display = [[] for b in range(ARRAY_WIDTH)]

def integrate(fourier, start, end):
    retval = 0
    for c in range(start, end):
        retval += fourier[c]
    return retval

def createWidth(fourier):
    max = int(len(fourier)*20000/samplingFrequency)
    interval = int(max/ARRAY_WIDTH)
    start, end = 0, interval
    for i in range(ARRAY_WIDTH):
        display[i] = integrate(fourier, start, end)
        start += interval
        end += interval

def norm():
    norm = 25000
    if max(display) > 25000:
        norm = max(display)
    return display * 8 / norm

def main():
    matrix = Matrix(8 * 8 * 4, 18, 800000, 5, 255, False, 50)
    matrix.clearMatrix()

    device = 'hw:1'
    inp = alsaaudio.PCM(alsaaudio.PCM_CAPTURE,
                        alsaaudio.PCM_NONBLOCK, device=device)

    # Set attributes: Mono, 44100 Hz, 16 bit little endian samples
    inp.setchannels(1)
    inp.setrate(44100)
    inp.setformat(alsaaudio.PCM_FORMAT_S16_LE)
    inp.setperiodsize(160)

    while True:
        # Read data from device
        l, data = inp.read()
        signal = []

        for d in data:
            signal.append(d)

        fourier = fft(signal)
        createWidth(fourier)
        matrix.arrayToMatrix(display)

if __name__ == "__main__":
    main()