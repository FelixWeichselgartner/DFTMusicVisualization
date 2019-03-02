## DFTMusicVisualization
=====================

music visualization on a rgb led matrix with fourier transformation


To install the library for the matrix communication run the following in the lib folder:
```
git clone https://github.com/jgarff/rpi_ws281x.git && cd rpi_ws281x
```
At first you will have to install scons.
```
sudo apt-get install scons
```
Then run `scons` to install the library.
After that you will have to delete the main.c file in the new folder lib/rpi_ws281x


Then run:
```
cmake . && make
```
in the root directory.


execute the program with:
```
sudo ./DFTMusicVisualization
```


## What I Learned

* implementation of fourier transformation
* controlling led matrices with raspberrypi/arduino
* using CMake
* function principle behind audio files
* sampling audio files