swig -python dft.i
gcc -O2 -fPIC -c dft.c -lm
gcc -O2 -fPIC -c dft_wrap.c -I/usr/include/python3.7
gcc -shared dft.o dft_wrap.o -o _dft.so -lm