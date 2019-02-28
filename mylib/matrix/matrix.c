/*
this file is oriented on:
https://github.com/jgarff/rpi_ws281x/blob/master/main.c
*/

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

//unknown so far - this is changed later
#define LED_COUNT 0

int width, height, led_count;

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
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            matrix[y * width + x] = 0;
        }
    }
}

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

void OnOff(int i, int v) {
    if (v == 0 || v > width || v < 0) {
        for (int k = 0; k < width; k++) {
            matrix[i * width + k] = 0;
        }
    } else {
        for (int k = 0; k < v; k++) {
            matrix[i * width + k] = dotcolors[0];
        }
        for (int f = v; f < width; f++) {
            matrix[i * width + f] = 0;
        }
    }
}

void writeDisplayMatrix(int *display) {
    for (int i = 0; i < width; i++) {
        OnOff(i, display[i]);
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

static ws2811_return_t ret;

void myMatrixSetup(int w, int h) {
    width = w;
    height = h;
    led_count = width * height;
    ledstring.channel[0].count = led_count;
    matrix = malloc(sizeof(ws2811_led_t) * led_count);
    setup_handlers();

    if ((ret = ws2811_init(&ledstring)) != WS2811_SUCCESS) {
        fprintf(stderr, "ws2811_init failed: %s\n", ws2811_get_return_t_str(ret));
        return;
    }
}

void myMatrixOutput(int *display) {
    writeDisplayMatrix(display);
    matrix_render();

    if ((ret = ws2811_render(&ledstring)) != WS2811_SUCCESS) {
        fprintf(stderr, "ws2811_render failed: %s\n", ws2811_get_return_t_str(ret));
        return;
    }

    return;
}

void myMatrixEnd() {
    matrix_clear();
    matrix_render();
    ws2811_render(&ledstring);
    free(matrix);
    ws2811_fini(&ledstring);
}
