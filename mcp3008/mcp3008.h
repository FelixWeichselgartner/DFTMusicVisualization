#ifndef MCP3008_H_
#define MCP3008_H_

const int CHAN_CONFIG_SINGLE = 8, CHAN_CONFIG_DIFF = 0;

void loadSpiDriver();

int spiSetup(int);

int mcpAnalogRead(int, int, int);

#endif
