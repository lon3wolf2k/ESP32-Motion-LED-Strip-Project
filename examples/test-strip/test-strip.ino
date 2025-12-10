#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel s(10,5,NEO_GRB+NEO_KHZ800);
void setup(){s.begin();}
void loop(){for(int i=0;i<10;i++)s.setPixelColor(i,s.Color(255,0,0));s.show();delay(500);for(int i=0;i<10;i++)s.setPixelColor(i,0);s.show();delay(500);} 