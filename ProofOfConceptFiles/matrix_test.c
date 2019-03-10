/*
this is based on main.c of the repo:
https://github.com/jgarff/rpi_ws281x.git

Usage:
The API is very simple. 
Make sure to create and initialize 
the ws2811_t structure as seen in main.c. 
From there it can be initialized by calling ws2811_init(). 
LEDs are changed by modifying the color 
in the .led[index] array and calling ws2811_render(). 
The rest is handled by the library, which either 
creates the DMA memory and starts the DMA for PWM 
and PCM or prepares the SPI transfer buffer and 
sends it out on the MISO pin.

Make sure to hook a signal handler for SIGKILL to do cleanup. 
From the handler make sure to call ws2811_fini(). 
It'll make sure that the DMA is finished before 
program execution stops and cleans up after itself.
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
#include <wiringPi.h>

#include "../lib/rpi_ws281x/clk.h"
#include "../lib/rpi_ws281x/gpio.h"
#include "../lib/rpi_ws281x/dma.h"
#include "../lib/rpi_ws281x/pwm.h"
#include "../lib/rpi_ws281x/version.h"
#include "../lib/rpi_ws281x/ws2811.h"

#define true 1
#define false 0

#define ARRAY_SIZE(stuff)       (sizeof(stuff) / sizeof(stuff[0]))

// defaults for cmdline options
#define TARGET_FREQ             WS2811_TARGET_FREQ
#define GPIO_PIN                18
#define DMA                     10
#define STRIP_TYPE              WS2811_STRIP_GBR		// WS2812/SK6812RGB integrated chip+leds

#define WIDTH                   8 * 3
#define HEIGHT                  8
#define LED_COUNT               (WIDTH * HEIGHT)

int running = 1;

int width = WIDTH;
int height = HEIGHT;
int led_count = LED_COUNT;
int currentled;

int clear_on_exit = 0;

ws2811_t ledstring =
{
    .freq = TARGET_FREQ,
    .dmanum = DMA,
    .channel =
    {
        [0] =
        {
            .gpionum = GPIO_PIN,
            .count = LED_COUNT,
            .invert = 0,
            .brightness = 255,
            .strip_type = STRIP_TYPE,
        },
        [1] =
        {
            .gpionum = 0,
            .count = 0,
            .invert = 0,
            .brightness = 0,
        },
    },
};

ws2811_led_t *matrix;

ws2811_led_t dotcolors[] =
{
    0x00200000,  // red
    0x00201000,  // orange
    0x00202000,  // yellow
    0x00002000,  // green
    0x00002020,  // lightblue
    0x00000020,  // blue
    0x00100010,  // purple
    0x00200010,  // pink
};

void matrix_set() {
    if (currentled - 1 < 0) {
        matrix[led_count - 1] = 0;
    } else {
        matrix[currentled - 1] = 0;
    }
    if (currentled >= led_count) {
        currentled = 0;
    }
    matrix[currentled] = dotcolors[7];
    currentled++;
}

void matrix_render() {
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            ledstring.channel[0].leds[(y * width) + x] = matrix[y * width + x];
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

/**
 * @brief  test for the matrix
 * @note   lets each led light up individually -> so you can see the arangement of the leds
 * @retval 
 */
int main() {
    currentled = 0;
    printf("you are running the matrix_test\n");
    short running = true;
    int sleeptime = 1000000 / 15;

    matrix = malloc(sizeof(ws2811_led_t) * width * height);
    if (matrix == NULL) {
	    printf("failed to get heap memory\n");
    } else {
	    printf("heap memory was reserved\n");
    }

    setup_handlers();

    ws2811_return_t ret;
    ret = ws2811_init(&ledstring);
    if (ret != WS2811_SUCCESS) {
        printf("failed to initialize\n");
        return 1;
    } else {
	    printf("initialized the led string");
    }

    int count = 0;

    while (running) { 
        printf("loop\n");
        count++;
        if (count == 2 * width * height + height)
            break;
        matrix_set();
        matrix_render();

        if ((ret = ws2811_render(&ledstring)) != WS2811_SUCCESS) {
            printf("failed to render\n");
            break;
        }
        
        delay(50);
    }

    matrix_clear();
    matrix_render();
    ws2811_fini(&ledstring);
    printf("program closed\n");

    return 0;
}
