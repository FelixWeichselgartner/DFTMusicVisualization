#ifndef MCP3008_H_
#define MCP3008_H_

extern int spi;

const int CHAN_CONFIG_SINGLE = 8, CHAN_CONFIG_DIFF = 0;

void loadSpiDriver();

int spiSetup(int);

int analogRead(int, int, int);

#endif