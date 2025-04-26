#ifndef globaldefs
  #define globaldefs

  #include <Arduino.h>
  #include <FastLED.h>

  #define NUM_LEDS 80

  #define HEIGHT (NUM_LEDS/4)

  #define LED_PIN 13
  #define USER_BUTTON 0

  CRGB g_LEDs[NUM_LEDS] = {0};

  #define LED_BLUE 2

  static int pattern = 3;

  void NextPattern() {
    pattern = (pattern + 1) % 5;
  }

  void setPattern(int newPattern) {
    Serial.print("setPattern ");
    Serial.println(newPattern);
    pattern = newPattern % 5;
  }

#endif
