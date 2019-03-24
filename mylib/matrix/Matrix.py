from neopixel import *
import time

class Matrix(object):

    def __init__(
        self, 
        led_count, 
        led_pin, 
        led_freq_hz, 
        led_dma,
        led_brightness,
        led_invert,
        wait_ms
        ):

        self.led_count = led_count
        self.led_pin = led_pin
        self.led_freq_hez = led_freq_hz
        self.led_dma = led_dma
        self.led_brightness = led_brightness
        self.led_invert = led_invert
        self.wait_ms = wait_ms

        self.strip = Adafruit_NeoPixel(
            led_count,
            led_pin,
            led_freq_hz,
            led_dma,
            led_invert,
            led_brightness
            )
        
        strip.begin()

    def clearMatrix():
        for i in range(strip.numPixels()):
            strip.setPixelColor(i, Color(0, 0, 0))
            strip.show()
            time.sleep(wait_ms/1000)
    
    def arrayToMatrix(arr):
        widthOneMatrix = 8
        r, start, end = 0, 0, 0
        for i in range(width/widthOneMatrix, 0, -1):
            start = (n - 1) * widthOneMatrix * widthOneMatrix + widthOneMatrix - 1
            end = (n - 1) * widthOneMatrix * widthOneMatrix
            b = (width / 8 - n) * widthOneMatrix
            for k in range(start, end, -1):
                for l in range(height):
                    if display[b] > i:
                        strip.setPixelColor(i * widthOneMatrix + k, Color(255, 0, 0))
                    else:
                        strip.setPixelColor(i * widthOneMatrix + k, Color(0, 0, 0))
                b += 1
