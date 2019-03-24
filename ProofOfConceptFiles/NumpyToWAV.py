import numpy as np
from scipy.io.wavfile import write

def ConvertNumpyToWav(name, data, frequency):
    scaled = np.int16(data/np.max(np.abs(data)) * 32767)
    write(name + '.wav', frequency, scaled)

def ConvertListToWav(name, theList):
    arr = np.zeros(len(theList))
    count = 0
    for i in theList:
        arr[count] = i
        count += 1
		
    try:
	    ConvertNumpyToWav(name, arr, 30000)
    except:
	    sys.exit("usage: python3 NumpyToWAV.py <sampling frequency>")


def main():
    signal = []
    print(signal)
    with open('sample.csv') as f:
        # with: i = index and v = value
        for i, v in enumerate(f):
            signal.append(v)
            if (i < 16):
                print(f'{i}: {signal[i]} == {v}')
        ConvertListToWav('sample_python', signal)

if __name__ == "__main__":
    main()