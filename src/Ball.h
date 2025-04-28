#ifndef balldefs
#define balldefs

  #include <algorithm>
  #include <iterator>
  #include <FastLED.h>
  #include "global.h"

  #define BALLSIZE (kMatrixHeight / 3)

  extern CRGB g_LEDs[];

  CRGB frontLEDs[kMatrixWidth] = {0};
  CRGB backLEDs[kMatrixWidth] = {0};
  CRGB leftLEDs[kMatrixWidth] = {0};
  CRGB rightLEDs[kMatrixWidth] = {0};

  bool BallOutOfBounds(float x) {
    bool retVal = (x < 0.0) || ((x + (float)BALLSIZE) > (float)kMatrixWidth);
    return retVal;
  }

  float BallNormalize(float x) {
    float retVal = 0.0;
    if (x < 0.0) {
      return retVal;
    }
    if ((x + (float)BALLSIZE) > kMatrixWidth) {
      retVal = (float)kMatrixWidth - (float)BALLSIZE;
      return retVal;
    }

    return(x);
  }

  void DrawBall() {
    static byte hue = HUE_RED;
    const int deltaHue = 3;

    static float x = 10.0;
    static float dx = (float) kMatrixWidth / 100.0;

    x += dx;
    if (BallOutOfBounds(x)){
      dx = -dx;
      x = BallNormalize(x);
    }

    hue += deltaHue;
    for (int i = 0; i < kMatrixWidth; i++) {
      frontLEDs[i] = frontLEDs[i].fadeToBlackBy(40);
      backLEDs[i] = backLEDs[i].fadeToBlackBy(40);
      leftLEDs[i] = leftLEDs[i].fadeToBlackBy(40);
      rightLEDs[i] = rightLEDs[i].fadeToBlackBy(40);
    }

    for (int i = (int)x ; i < BALLSIZE + (int)x; i++){
      frontLEDs[i].setHue(hue);
      backLEDs[kMatrixWidth - i - 1].setHue(hue);
      leftLEDs[i].setHue(hue);
      rightLEDs[kMatrixWidth - i - 1].setHue(hue);
    }

    for (int i = 0; i < kMatrixWidth; i++) {
      g_LEDs[i + 0 * kMatrixWidth] = frontLEDs[i];
      g_LEDs[i + 1 * kMatrixWidth] = backLEDs[i];
      g_LEDs[i + 2 * kMatrixWidth] = leftLEDs[i];
      g_LEDs[i + 3 * kMatrixWidth] = rightLEDs[i];
    }

    blur2d(g_LEDs, kMatrixWidth, kMatrixHeight, 12, xymap);
    FastLED.show();
    delay(20);
  }

#endif
