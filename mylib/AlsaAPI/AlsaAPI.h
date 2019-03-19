#ifndef ALSAAPI_H_
#define ALSAAPI_H_

#include <stdio.h>
#include <stdlib.h>
#include <alsa/asoundlib.h>

int initAlsa(int, int, int, int);
int readAlsa(short **);
void closeAlsa();

#endif
