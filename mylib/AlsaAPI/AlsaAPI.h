#ifndef ALSAAPI_H_
#define ALSAAPI_H_

#include <stdio.h>
#include <stdlib.h>
#include <alsa/asoundlib.h>

int init(int, int, int, int);
void read(unsigned short *, int);
void close();

#endif