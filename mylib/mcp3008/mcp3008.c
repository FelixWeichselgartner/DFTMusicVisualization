#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <errno.h>

/**
 * @brief  loads the spi drivers
 * @note   
 * @retval None
 */
void loadSpiDriver() {
	if (system("gpio load spi") == -1) {
		fprintf(stderr, "Can't load the SPI driver: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
}

/**
 * @brief  				sets up the spi
 * @note   
 * @param  spiChannel: 	the current spi channel
 * @retval 
 */
int spiSetup(int spiChannel) {
    int spiClockSpeed = 1000000, spi;
	if ((spi = wiringPiSPISetup(spiChannel, spiClockSpeed)) < 0) {
		printf("Can't open the SPI bus: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
    return spi;
}

/**
 * @brief  					reads an analog value from the mcp3008
 * @note   
 * @param  spiChannel: 		the current spi channel
 * @param  channelConfig: 	the current channel config
 * @param  analogChannel: 	the current analog channel (for the mcp3008: [0;7])
 * @retval 					the read analog value
 */
int mcpAnalogRead(int spiChannel, int channelConfig, int analogChannel) {
	unsigned char buffer[3] = { 1 };
	buffer[1] = (channelConfig + analogChannel) << 4;
	wiringPiSPIDataRW(spiChannel, buffer, 3);
	return ((buffer[1] & 3) << 8) + buffer[2];
}
