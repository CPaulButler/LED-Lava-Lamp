#ifndef balldefs
#define balldefs

  #include <algorithm>
  #include <iterator>
  #include <FastLED.h>
  #include "global.h"

  extern CRGB g_LEDs[];

  CRGB frontLEDs[HEIGHT] = {0};
  CRGB backLEDs[HEIGHT] = {0};
  CRGB leftLEDs[HEIGHT] = {0};
  CRGB rightLEDs[HEIGHT] = {0};

  bool BallOutOfBounds(float x) {
    bool retVal = (x < 0.0) || ((x + (float)BALLSIZE) > (float)HEIGHT);
    return retVal;
  }

  float BallNormalize(float x) {
    float retVal = 0.0;
    if (x < 0.0) {
      return retVal;
    }
    if ((x + (float)BALLSIZE) > HEIGHT) {
      retVal = (float)HEIGHT - (float)BALLSIZE;
      return retVal;
    }

    return(x);
  }

  void DrawBall() {
    static byte hue = HUE_RED;
    const int deltaHue = 3;

    static float x = 10.0;
    static float dx = (float) HEIGHT / 100.0;

    x += dx;
    if (BallOutOfBounds(x)){
      dx = -dx;
      x = BallNormalize(x);
    }

    hue += deltaHue;
    for (int i = 0; i < HEIGHT; i++) {
      frontLEDs[i].setColorCode(0);
      backLEDs[i].setColorCode(0);
      leftLEDs[i].setColorCode(0);
      rightLEDs[i].setColorCode(0);
    }

    for (int i = (int)x ; i < BALLSIZE + (int)x; i++){
      frontLEDs[i].setHue(hue);
      backLEDs[HEIGHT - i - 1].setHue(hue);
      leftLEDs[i].setHue(hue);
      rightLEDs[HEIGHT - i - 1].setHue(hue);
    }

    FastLED.clear();
    for (int i = 0; i < HEIGHT; i++) {
      g_LEDs[i + 0 * HEIGHT] = frontLEDs[i];
      g_LEDs[i + 1 * HEIGHT] = backLEDs[i];
      g_LEDs[i + 2 * HEIGHT] = leftLEDs[i];
      g_LEDs[i + 3 * HEIGHT] = rightLEDs[i];
    }

    FastLED.show();
    delay(20);
  }

#endif
