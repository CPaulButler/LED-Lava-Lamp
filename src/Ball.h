#ifndef balldefs
#define balldefs

  #include <algorithm>
  #include <iterator>
  #include <FastLED.h>
  #include "global.h"
  CRGB frontLEDs[HEIGHT] = {0};
  CRGB backLEDs[HEIGHT] = {0};
  CRGB leftLEDs[HEIGHT] = {0};
  CRGB rightLEDs[HEIGHT] = {0};

  bool OutOfBounds(float x) {
    return (x < 0.0) || ((x + BALLSIZE) > (float)HEIGHT);
  }

  float Normalize(float x) {
    if (x < 0.0) return 0.0;
    if ((x + (float)BALLSIZE) > HEIGHT) return (float)(HEIGHT - BALLSIZE);
  }

  void DrawBall() {
    static byte hue = HUE_RED;
    const int deltaHue = 3;

    static float x = 0;
    static float dx = (float) HEIGHT / 100.0;

    x = x + dx;
    if (OutOfBounds(x)) dx = -dx;
    x = Normalize(x);

      // Randomly fade the LEDs
      for (int j = 0; j < NUM_LEDS; j++)
        g_LEDs[j] = g_LEDs[j].fadeToBlackBy(127);

    hue += deltaHue;
    for (int i = (int)x ; i <= BALLSIZE + (int)x; i++){
      frontLEDs[i].setHue(hue);
      backLEDs[HEIGHT - i - 1].setHue(hue);
      leftLEDs[i].setHue(hue);
      rightLEDs[HEIGHT - i - 1].setHue(hue);
    }

    for (int i = 0; i < NUM_LEDS; i++) {
      g_LEDs[i].setHue(hue);
    }

    FastLED.show();
    delay(20);
  }

#endif
