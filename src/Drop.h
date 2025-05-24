
#ifndef dropdefs
#define dropdefs

  #include <FastLED.h>
  #include "global.h"

  struct litLED {
    float x, y, dx;
    CRGB c;
  };

  #define litLEDCount 3
  static litLED litLEDs[litLEDCount];
  static int nextLED = 1;

  litLED updateLED(litLED l) {
    litLED retL;
    retL.x = l.x + l.dx;
    retL.y = l.y;
    retL.c = l.c; //.fadeToBlackBy(20);
    if (retL.x < 0.0) {
      /*
      Serial.printf("%4.1f < 0, ", retL.x);
      Serial.printf("x = %4.1f, y = %4.1f, r = %3d, g = %3d, b = %3d\n", retL.x, retL.y, retL.c.r, retL.c.g, retL.c.b);
      */
      retL.c = CRGB::Black;
    }
    return retL;
  }

  uint8_t fade (litLED l) {
    const float fadeSlope = 7.0;
    float distanceFromBottom = l.x;
    if (distanceFromBottom <= fadeSlope) {
      uint8_t rval = (uint8_t) ((fadeSlope - distanceFromBottom) / fadeSlope * 255.0);
      //Serial.printf("l.x = %4.1f, distanceFromBottom = %4.1f, rval = %d\n", l.x, distanceFromBottom, rval);
      return (uint8_t) ((fadeSlope - distanceFromBottom) / fadeSlope * 255.0);
    }

    float distanceFromTop = ((float) kMatrixWidth - 1) - l.x;
    if (distanceFromTop <= fadeSlope) {
      uint8_t rval = (uint8_t) ((fadeSlope - distanceFromTop) / fadeSlope * 255.0);
      //Serial.printf("l.x = %4.1f, distanceFromTop = %4.1f, rval = %d\n", l.x, distanceFromTop, rval);
      return rval;
    }

    //Serial.printf("l.x = %4.1f\n", l.x);
    return 0;
  }

  int emptyLED() {
    for (int i = 0; i < litLEDCount; i++){
      if (litLEDs[i].x <= 0) {
        //Serial.printf("empty at %d\n",i);
        return i;
      }
    }
    //Serial.println("none empty");
    return -1;
  }

  void DrawDrop() {
    float x = random(kMatrixWidth - 1);
    float dx;
    float y = random(kMatrixHeight - 1);
    CRGB c = CRGB(0, 0, 0);

    EVERY_N_MILLISECONDS(300) {
      x = (float) (kMatrixWidth - 1);
      dx = -((float) random(100)) / 150.0 - 0.50;
      y = random(kMatrixHeight - 1);
      uint8_t r = random(128);
      uint8_t g = random(128);
      uint8_t b = random(128);
      c = CRGB(r, g, b);

      nextLED = emptyLED();
      if (nextLED >= 0) {
        //Serial.printf("Adding LED at %d\n", nextLED);
        litLEDs[nextLED] = {x, y, dx, c};
      }

    }

    FastLED.clear();
    for (int i = 0; i < litLEDCount; i++)
    {
      litLED iLED = litLEDs[i];
      //Serial.printf("i = %d, x = %4.1f, y = %4.1f, r = %3d, g = %3d, b = %3d\n", i, iLED.x, iLED.y, iLED.c.r, iLED.c.g, iLED.c.b);
      g_LEDs[xymap((int) iLED.x,(int) iLED.y)] += iLED.c; //.fadeToBlackBy(fade(iLED));
      g_LEDs[xymap((int) iLED.x,(int) iLED.y)].fadeToBlackBy(fade(iLED));
      litLEDs[i] = updateLED(iLED);
    }

    // blur2d(g_LEDs, kMatrixWidth, kMatrixHeight, 7, xymap);

    FastLED.show();
    delay(50);
  }

#endif
