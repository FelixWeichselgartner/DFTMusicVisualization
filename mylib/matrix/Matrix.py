from lib.rpi_ws281x.python.neopixel import *
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
        
        self.strip.begin()

    def clearMatrix(self):
        #print('led count is ' + str(self.led_count))
        for i in range(self.led_count):
            #print(i)
            self.strip.setPixelColor(i, Color(0, 0, 0))
            #self.strip.show()
            #time.sleep(self.wait_ms/1000)
        self.strip.show()
    
    def arrayToMatrix(self, arr, amount, width, height, widthOneMatrix):
        #self.clearMatrix()
        r, start, end = 0, 0, 0
        for i in range(amount, 0, -1):
            start = (i - 1) * widthOneMatrix * widthOneMatrix + widthOneMatrix - 1
            end = (i - 1) * widthOneMatrix * widthOneMatrix - 1
            b = (amount - i) * widthOneMatrix
            for k in range(start, end, -1):
                for l in range(height):
                    if arr[b] > l:
                        self.strip.setPixelColor(l * widthOneMatrix + k, Color(66, 235, 244))
                    else:
                        self.strip.setPixelColor(l * widthOneMatrix + k, Color(0, 0, 0))
                #self.strip.show()
                #time.sleep(self.wait_ms/1000)
                b += 1
        self.strip.show()

    def setBrightness(self, value):
        self.strip.setBrightness(value)
