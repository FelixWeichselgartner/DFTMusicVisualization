*****************************************************************************************

## DFTMusicVisualization

music visualization on a (acutally four) rgb led matrix with fourier transformation

Here a small gif that shows the output:

![gif][https://github.com/FelixWeichselgartner/DFTMusicVisualization/blob/master/resources/MyOutput.gif]

*****************************************************************************************

### Copyright 2019 Felix Weichselgartner

All rights are reserved.

If you have questions or improvements pls contact me:

info@felix-weichselgartner.de

*****************************************************************************************

### Installation

To install the library for the matrix communication run the following in the lib folder:
```
git clone https://github.com/jgarff/rpi_ws281x.git && cd rpi_ws281x
```

At first you will have to install scons.
```
sudo apt-get install scons
```

To install the library run:
```
scons
```

After that you will have to delete the main.c file in the new folder lib/rpi_ws281x
```
rm main.c
```

Then install the python files with:
```
cd python
sudo python3 setup.py build
sudo python3 setup.py install
```

execute the python script (working) with:
```
sudo python3 DFTMusicVisualization.py
```

To execute the C-program run
```
cmake . && make
```
then:
```
sudo ./DFTMusicVisualization
```

execute the C-program (currently audio import not working) with:
```
sudo ./DFTMusicVisualization
```

*****************************************************************************************

## What I Learned

* implementation of fourier transformation
* controlling led matrices with raspberrypi/arduino
* using CMake
* function principle behind audio files
* sampling audio files
* working with ALSA (Advanced Linux Sound Architecture)

*****************************************************************************************
