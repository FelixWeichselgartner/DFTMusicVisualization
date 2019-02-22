#include <FastLED.h>
#define NUM_LEDS 192
#define DATA_PIN 6
#define Helligkeit1 10
#define Helligkeit2 20
#define laenge 8
#define hoehe 8

CRGB leds[NUM_LEDS];

int array[12]={8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8};

void setup() { 
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.clear(true);
  //FastLED.setBrightness(Helligkeit2);
}

void ausgabe(int *array){
  for(int i=0; i<12; i++){
    if(array[i]!=0){
      for(int j=0; j<array[i]; j++){
        leds[j+i*12]=CRGB::Blue;
        leds[24*8-j]=CRGB::Blue;
     }
     for(int j=array[i]; j<8; j++){
       leds[j+i*12]=CRGB::Black;
       leds[24*8-j]=CRGB::Blue;
      }
   }
   if(array[i]==0){
    for(int j=0; j<8; j++){
     leds[i*12]=CRGB::Black;
     leds[24*8-i*12]=CRGB::Black;
     }
   }
  }
}

void loop() { 
    ausgabe(array);
    FastLED.show();
    
}
