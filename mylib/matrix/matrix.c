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
#include <time.h>

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

/**
 * @brief           copies the array for the matrix to the matrix
 * @note   
 * @retval None
 */
void matrix_render() {
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            ledstring.channel[0].leds[y * width + x] = matrix[y * width + x];
        }
    }
}

/**
 * @brief           sets all values of the array for the matrix to 0 (off)
 * @note   
 * @retval None
 */
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

/**
 * @brief               converts the display array into the corresponding matrix array
 * @note   
 * @param  *display:    pointer to the display array -> contains the height for each column of the matrix
 * @retval None
 */
void writeDisplayMatrix(int *display) {
    short MATRIX_DEBUG = 0;
    matrix_clear();
    short widthOneMatrix = 8;
    srand(time(NULL));
    int r, start, end;
    for  (int n = width/widthOneMatrix; n > 0; n--) {
        start = (n - 1) * widthOneMatrix * widthOneMatrix + widthOneMatrix - 1;
        end = (n - 1) * widthOneMatrix * widthOneMatrix;
        if (MATRIX_DEBUG)
            printf("n = %i, start = %i, end = %i\n", n, start, end);
        for (int k = start, b = (width / 8 - n) * widthOneMatrix; k >= end; k--, b++) {
            if (MATRIX_DEBUG)
            printf("\tk = %2i, b = %2i\n", k, b);
            for (int i = 0; i < height; i++) {
                if (MATRIX_DEBUG)
                    printf("\t\ti = %2i, i * 8 + k = %2i\t%i", i, i * widthOneMatrix + k, display[b]);
                if (display[b] > i) {
                    matrix[i * widthOneMatrix + k] = dotcolors_rgbw[4];
                    if (MATRIX_DEBUG)
                        printf("\ton\n");
                } else {
                    matrix[i * widthOneMatrix + k] = 0;
                    if (MATRIX_DEBUG)
                        printf("\toff\n");
                }
            }
        }
    }
}

static ws2811_return_t ret;

/**
 * @brief           sets up the matrix communication
 * @note            initialises the width, height and a ledstring (the matrices) and reserves memory for 
 * @param  w:       width of the matrix
 * @param  h:       height of the matrix
 * @retval None
 */
void myMatrixSetup(int w, int h) {
    width = w;
    height = h;
    led_count = width * height;
    ledstring.channel[0].count = led_count;
    matrix = malloc(sizeof(ws2811_led_t) * led_count);

    if ((ret = ws2811_init(&ledstring)) != WS2811_SUCCESS) {
        fprintf(stderr, "ws2811_init failed: %s\n", ws2811_get_return_t_str(ret));
        return;
    }
}

/**
 * @brief               writes the display array to a array for the matrix
 * @note   
 * @param  *display:    pointer to the display array -> contains the height for each column of the matrix
 * @retval None
 */
void myMatrixOutput(int *display) {
    writeDisplayMatrix(display);
    matrix_render();

    if ((ret = ws2811_render(&ledstring)) != WS2811_SUCCESS) {
        fprintf(stderr, "ws2811_render failed: %s\n", ws2811_get_return_t_str(ret));
        return;
    }
    
    return;
}

/**
 * @brief               clears the matrix, frees memory, ends communication
 * @note   
 * @retval None
 */
void myMatrixEnd() {
    matrix_clear();
    matrix_render();
    ws2811_render(&ledstring);
    free(matrix);
    ws2811_fini(&ledstring);
}
