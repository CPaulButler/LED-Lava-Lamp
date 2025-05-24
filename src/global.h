#ifndef globaldefs
  #define globaldefs

  #include <Arduino.h>
  #include <FastLED.h>
  #include <fl/xymap.h>
  #include "modes.h"

  // Params for width and height
  const uint8_t kMatrixWidth = 20;
  const uint8_t kMatrixHeight = 4;

  #define NUM_LEDS (kMatrixWidth * kMatrixHeight)
  #define LAST_VISIBLE_LED 79

  fl::XYMap xymap(kMatrixWidth, kMatrixHeight);

  #define LED_PIN 13
  #define USER_BUTTON 0

  CRGB g_LEDs[NUM_LEDS] = {0};

  #define LED_BLUE 2

  RTC_DATA_ATTR static int pattern = Drop;
  static int lastPattern = Comet;

  void NextPattern() {
    pattern = (pattern + 1) % ModeCount;
    Serial.print("setPattern ");
    Serial.println(pattern);
  }

  void setPattern(int newPattern) {
    Serial.print("setPattern ");
    Serial.println(newPattern);
    pattern = newPattern % ModeCount;
  }

#endif
