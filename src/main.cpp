#include <Arduino.h>
#include <FastLED.h>
#include <WiFi.h>
#include <chsv.h>
#include "Elastic.h"
#include "comet.h"
#include "WebStuff.h"
#include "Ball.h"
#include "RandomBlur.h"
#include "driver/touch_sensor_common.h"

void IRAM_ATTR buttonISR() {
  NextPattern();
  FastLED.clear();
}

uint32_t lastTouch = 0;

void touchISR() {
  if (millis() - lastTouch > 1000) {
    lastTouch = millis();
    NextPattern();
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println("Hello WOrld");

  touch_high_volt_t highV;
  touch_low_volt_t lowV;
  touch_volt_atten_t attenV;
  esp_err_t err;
  err = touch_pad_get_voltage(&highV, &lowV, &attenV);
  highV = TOUCH_HVOLT_2V4;
  lowV = TOUCH_LVOLT_0V8;
  attenV = TOUCH_HVOLT_ATTEN_1V5;
  err = touch_pad_set_voltage(highV, lowV, attenV);

  touchAttachInterrupt(T6, touchISR, 50);

  touchSleepWakeUpEnable(T6, 50);

  webSetup(setPattern);

  // put your setup code here, to run once:
  // int result = myFunction(2, 3);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(USER_BUTTON, INPUT);
  attachInterrupt(USER_BUTTON, buttonISR, FALLING);

  FastLED.addLeds<WS2812B, LED_PIN, GRB>(g_LEDs, NUM_LEDS).setScreenMap(xymap);;
  FastLED.setBrightness(100);
  set_max_power_indicator_LED(LED_BLUE);                               // Light the builtin LED if we power throttle
  FastLED.setMaxPowerInMilliWatts(400);                          // Set the power limit, above which brightness will be throttled
}

uint8_t initialHue = 0;
const uint8_t deltaHue = 4;
const uint8_t hueDensity = 3;

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
    NextPattern();
    delay(500);
    esp_deep_sleep_start();
  } else if (Random == pattern) {
    DrawRandom();
  }

  //fill_gradient(g_LEDs, NUM_LEDS, CHSV(HSVHue::HUE_PURPLE, 255, 255), CHSV(HSVHue::HUE_YELLOW, 255, 255), SHORTEST_HUES);
}

