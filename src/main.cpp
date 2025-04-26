#include <Arduino.h>
#include <FastLED.h>
#include <WiFi.h>
#include <chsv.h>
#include "Elastic.h"
#include "comet.h"
#include "WebStuff.h"
#include "Ball.h"

void IRAM_ATTR buttonISR() {
  NextPattern();
  FastLED.clear();
}

void setup() {
  Serial.begin(9600);
  Serial.println("Hello WOrld");

  webSetup(setPattern);

  // put your setup code here, to run once:
  // int result = myFunction(2, 3);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(USER_BUTTON, INPUT);
  attachInterrupt(USER_BUTTON, buttonISR, FALLING);

  FastLED.addLeds<WS2812B, LED_PIN, GRB>(g_LEDs, NUM_LEDS);
  FastLED.setBrightness(100);
  set_max_power_indicator_LED(LED_BLUE);                               // Light the builtin LED if we power throttle
  FastLED.setMaxPowerInMilliWatts(400);                          // Set the power limit, above which brightness will be throttled
}

uint8_t initialHue = 0;
const uint8_t deltaHue = 2;
const uint8_t hueDensity = 1;

void loop() {
  webLoop();
  // put your main code here, to run repeatedly:
  /*
  digitalWrite(LED_BLUE, HIGH);
  delay(100);
  digitalWrite(LED_BLUE, LOW);
  delay(100);
  */

  if (Spring == pattern)
    DrawElastic();
  else if (Rainbow == pattern) {
    fill_rainbow(g_LEDs, NUM_LEDS, initialHue += hueDensity, deltaHue);
    FastLED.show();
    delay(20);
  }
  else if (Comet == pattern) {
    DrawComet();
  } else if (Ball == pattern) {
    DrawBall();
  } else if (Off == pattern){
    FastLED.clear();
    FastLED.show();
    delay(100);
  }

  //fill_gradient(g_LEDs, NUM_LEDS, CHSV(HSVHue::HUE_PURPLE, 255, 255), CHSV(HSVHue::HUE_YELLOW, 255, 255), SHORTEST_HUES);
}

