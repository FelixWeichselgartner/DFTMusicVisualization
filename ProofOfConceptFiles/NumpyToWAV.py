import numpy as np
from scipy.io.wavfile import write

def ConvertNumpyToWav(name, data):
    scaled = np.int16(data/np.max(np.abs(data)) * 32767)
    write(name + '.wav', 44100, scaled)