#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <errno.h>

void loadSpiDriver() {
	if (system("gpio load spi") == -1) {
		fprintf(stderr, "Can't load the SPI driver: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
}

int spiSetup(int spiChannel) {
    int spiClockSpeed = 1000000, spi;
	if ((spi = wiringPiSPISetup(spiChannel, spiClockSpeed)) < 0) {
		printf("Can't open the SPI bus: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
    return spi;
}

int mcpAnalogRead(int spiChannel, int channelConfig, int analogChannel) {
	unsigned char buffer[3] = { 1 };
	buffer[1] = (channelConfig + analogChannel) << 4;
	wiringPiSPIDataRW(spiChannel, buffer, 3);
	return ((buffer[1] & 3) << 8) + buffer[2];
}