run the following to install the lib:
```
git clone https://github.com/jgarff/rpi_ws281x.git && cd rpi_ws281x
```
if you don't have it installed:
```
sudo apt-get install scons
```
then:
```
scons
```

in the end remove the main.c file so my CMake doesn't get confused
```
rm main.c
```