#include <Adafruit_NeoPixel.h>
#define PIR_PIN 14
#define LED_PIN 5
#define NUM_LEDS 90
#define ON_TIME 7000
#define FADE_STEPS 60
#define FADE_DELAY 10

Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);
unsigned long lastMotionTime = 0;
bool ledsAreOn = false;

uint8_t WW_R=255, WW_G=160, WW_B=80;

void setup(){
  pinMode(PIR_PIN, INPUT);
  strip.begin();
  strip.clear();
  strip.show();
}

void loop(){
  int motion = digitalRead(PIR_PIN);
  unsigned long now = millis();

  if(motion==HIGH){
    lastMotionTime=now;
    if(!ledsAreOn){ fadeIn(); ledsAreOn=true; }
  }

  if(ledsAreOn && (now-lastMotionTime>ON_TIME)){
    fadeOut(); ledsAreOn=false;
  }
}

void fadeIn(){
  for(int b=0;b<=255;b+=(255/FADE_STEPS)){
    setStripBrightness(b);
    delay(FADE_DELAY);
  }
  setStripBrightness(255);
}

void fadeOut(){
  for(int b=255;b>=0;b-=(255/FADE_STEPS)){
    setStripBrightness(b);
    delay(FADE_DELAY);
  }
  strip.clear(); strip.show();
}

void setStripBrightness(int brightness){
  float s=brightness/255.0;
  uint8_t r=WW_R*s, g=WW_G*s, b=WW_B*s;
  for(int i=0;i<NUM_LEDS;i++){
    strip.setPixelColor(i, strip.Color(r,g,b));
  }
  strip.show();
}
