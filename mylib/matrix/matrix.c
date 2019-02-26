#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <signal.h>
#include <stdarg.h>
#include <getopt.h>

#include "../../lib/rpi_ws281x/clk.h"
#include "../../lib/rpi_ws281x/gpio.h"
#include "../../lib/rpi_ws281x/dma.h"
#include "../../lib/rpi_ws281x/pwm.h"
#include "../../lib/rpi_ws281x/version.h"
#include "../../lib/rpi_ws281x/ws2811.h"


#define ARRAY_SIZE(stuff)       (sizeof(stuff) / sizeof(stuff[0]))

// defaults for cmdline options
#define TARGET_FREQ             WS2811_TARGET_FREQ
#define GPIO_PIN                18
#define DMA                     10
//#define STRIP_TYPE            WS2811_STRIP_RGB		// WS2812/SK6812RGB integrated chip+leds
#define STRIP_TYPE              WS2811_STRIP_GBR		// WS2812/SK6812RGB integrated chip+leds
//#define STRIP_TYPE            SK6812_STRIP_RGBW		// SK6812RGBW (NOT SK6812RGB)

#define WIDTH                   8
#define HEIGHT                  8
#define LED_COUNT               (WIDTH * HEIGHT)

int width = WIDTH;
int height = HEIGHT;
int led_count = LED_COUNT;

static uint8_t running = 1;

ws2811_t ledstring = {
    .freq = TARGET_FREQ,
    .dmanum = DMA,
    .channel = {
        [0] = {
            .gpionum = GPIO_PIN,
            .count = LED_COUNT,
            .invert = 0,
            .brightness = 255,
            .strip_type = STRIP_TYPE,
        },
        [1] = {
            .gpionum = 0,
            .count = 0,
            .invert = 0,
            .brightness = 0,
        },
    },
};

ws2811_led_t *matrix;

void matrix_render() {
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            ledstring.channel[0].leds[y * width + x] = matrix[y * width + x];
        }
    }
}

void matrix_clear() {
    for (int y = 0; y < (height ); y++) {
        for (int x = 0; x < width; x++) {
            matrix[y * width + x] = 0;
        }
    }
}

int dotspos[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
ws2811_led_t dotcolors[] = {
    0x00200000,  // red
    0x00201000,  // orange
    0x00202000,  // yellow
    0x00002000,  // green
    0x00002020,  // lightblue
    0x00000020,  // blue
    0x00100010,  // purple
    0x00200010,  // pink
};

ws2811_led_t dotcolors_rgbw[] = {
    0x00200000,  // red
    0x10200000,  // red + W
    0x00002000,  // green
    0x10002000,  // green + W
    0x00000020,  // blue
    0x10000020,  // blue + W
    0x00101010,  // white
    0x10101010,  // white + W

};

void writeDisplayMatrix(int *display) {
    for (int x = 0; x < width; x++) {
	for (int y = 0; y < height; y++) {
	    matrix[y * width + x] = dotcolors[0];
	}
    }
}

static void ctrl_c_handler(int signum) {
    (void)(signum);
    running = 0;
}

static void setup_handlers() {
    struct sigaction sa = {
        .sa_handler = ctrl_c_handler,
    };

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
}

void myMatrixOutput(int *display) {
    ws2811_return_t ret;

    matrix = malloc(sizeof(ws2811_led_t) * width * height);

    setup_handlers();

    if ((ret = ws2811_init(&ledstring)) != WS2811_SUCCESS) {
        fprintf(stderr, "ws2811_init failed: %s\n", ws2811_get_return_t_str(ret));
        return;
    }

    writeDisplayMatrix(display);
    matrix_render();

    if ((ret = ws2811_render(&ledstring)) != WS2811_SUCCESS) {
	fprintf(stderr, "ws2811_render failed: %s\n", ws2811_get_return_t_str(ret));
	return;
    }

    usleep(1000000 / 15);

    matrix_clear();
    matrix_render();
    ws2811_render(&ledstring);

    free(matrix);
    ws2811_fini(&ledstring);

    printf ("\n");
    return;
}