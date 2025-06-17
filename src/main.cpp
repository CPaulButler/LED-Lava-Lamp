#include <Arduino.h>
#include <FastLED.h>
#include <WiFi.h>
#include <chsv.h>
#include "Elastic.h"
#include "comet.h"
// #include "WebStuff.h"
#include "Ball.h"
#include "RandomBlur.h"
#include "Drop.h"
#include "ledgfx.h"
#include "fire.h"
#include "driver/touch_sensor_common.h"

void IRAM_ATTR buttonISR() {
  NextPattern();
  FastLED.clear();
}

uint32_t lastTouch = 0;

void touchISR() {
  if (millis() - lastTouch > 300) {
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

  touchAttachInterrupt(T6, touchISR, 60);

  touchSleepWakeUpEnable(T6, 60);

  // webSetup(setPattern);

  // put your setup code here, to run once:
  // int result = myFunction(2, 3);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(USER_BUTTON, INPUT);
  attachInterrupt(USER_BUTTON, buttonISR, FALLING);

  FastLED.addLeds<WS2812B, LED_PIN, GRB>(g_LEDs, NUM_LEDS).setScreenMap(xymap);;
  FastLED.setBrightness(255);
  set_max_power_indicator_LED(LED_BLUE);                               // Light the builtin LED if we power throttle

  // This seems to include a 125 mW allowance for the CPU. 500 mW is the max USB
  // allowed prior to negotiating with the host for more power.
  // Experimentally, using a USB power meter, a setting of 900 will limit total
  // power consumption to 100 mA * 5V = 0.5 W, which is the limit allowed for a
  // low-power device. This number really ought to be revalidated any time the
  // lighting pattern code is modified since it could lead to higher CPU power
  // consumption.
  FastLED.setMaxPowerInMilliWatts(900);                          // Set the power limit, above which brightness will be throttled
}

uint8_t initialHue = 0;
const uint8_t deltaHue = 4;
const uint8_t hueDensity = 3;

  #define cooling 35
  #define sparking 250
  ClassicFireEffect fire1(kMatrixWidth, 0, cooling, sparking, 1, 1, true, false);
  ClassicFireEffect fire2(kMatrixWidth, kMatrixWidth, cooling, sparking, 1, 1, false, false);
  ClassicFireEffect fire3(kMatrixWidth, 2 * kMatrixWidth, cooling, sparking, 1, 1, true, false);
  ClassicFireEffect fire4(kMatrixWidth, 3 * kMatrixWidth, cooling, sparking, 1, 1, false, false);

void loop() {
  // webLoop();
  unsigned long now;

  if (pattern != lastPattern) {
    FastLED.clear();
    FastLED.show();
    lastPattern = pattern;
  }

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
  } else if (Drop == pattern) {
    DrawDrop();
  } else if (Fire == pattern) {

    FastLED.clear();

    now = millis();
    fire1.DrawFire();
    fire2.DrawFire();
    fire3.DrawFire();
    fire4.DrawFire();
    // Serial.printf("fire compute %d\n",millis()-now);

    FastLED.show();
    delay(75);
  }

  // Turn off after 15 minutes without a touch
  if (millis() - lastTouch > 30 * 60 * 1000) {
    FastLED.clear();
    FastLED.show();
    esp_deep_sleep_start();
  }

}

