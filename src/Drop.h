
#ifndef dropdefs
#define dropdefs

  #include <FastLED.h>
  #include "global.h"

  struct litLED {
    float x, y, dx;
    CRGB c;
  };

  #define litLEDCount 4
  static litLED litLEDs[litLEDCount];
  static int nextLED = 1;

  litLED updateLED(litLED l) {
    litLED retL;

    retL.dx = l.dx;
    retL.y = l.y;
    retL.c = l.c; //.fadeToBlackBy(20);

    // Serial.printf("l.x = %4.1f, l.dx = %4.1f ", l.x, l.dx);
    retL.x = l.x + l.dx;
    if (retL.x < 0.0) {
      // Serial.printf("too  low ");
      retL.x = 0.0;
      retL.c = CRGB::Black;
    }

    if (retL.x > kMatrixWidth-1) {
      // Serial.printf("too high ");
      retL.x = kMatrixWidth - 1;
      retL.c = CRGB::Black;
    }

    // Serial.printf("retL.x = %4.1f\n", retL.x);
    return retL;
  }

  uint8_t fade (litLED l) {
    const float fadeSlope = 7.0;
    float distanceFromBottom = l.x;
    if (distanceFromBottom <= fadeSlope) {
      uint8_t rval = (uint8_t) ((fadeSlope - distanceFromBottom) / fadeSlope * 255.0);
      // Serial.printf("l.x = %4.1f, distanceFromBottom = %4.1f, rval = %d\n", l.x, distanceFromBottom, rval);
      return (uint8_t) ((fadeSlope - distanceFromBottom) / fadeSlope * 255.0);
    }

    float distanceFromTop = ((float) kMatrixWidth - 1) - l.x;
    if (distanceFromTop <= fadeSlope) {
      uint8_t rval = (uint8_t) ((fadeSlope - distanceFromTop) / fadeSlope * 255.0);
      // Serial.printf("l.x = %4.1f, distanceFromTop = %4.1f, rval = %d\n", l.x, distanceFromTop, rval);
      return rval;
    }

    // Serial.printf("l.x = %4.1f\n", l.x);
    return 0;
  }

  // emptyLED finds a place in litLEDs that is currently not showing a lit LED
  int emptyLED() {
    for (int i = 0; i < litLEDCount; i++){
      if ((litLEDs[i].x == 0.0) || (litLEDs[i].x == kMatrixWidth-1)) {
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

      // find an available spot in litLEDs and fill it with a new "active" light
      nextLED = emptyLED();
      if (nextLED >= 0) {
        dx = ((float) random(30)) / 100.0 + 0.10 ;
        if (random(100) > 85) dx = -dx;

        if (dx < 0.0) {
          x = (float) (kMatrixWidth - 1);
        } else {
          x = 0.0;
        }

        y = random(kMatrixHeight - 1);
        uint8_t r = random(255);
        uint8_t g = random(255);
        uint8_t b = random(255);
        c = CRGB(r, g, b);

        if (random(100)>66) {
          c = CRGB::Red;
        } else if (random(100)>50) {
          c = CRGB::Green;
        } else  {
          c = CRGB::Blue;
        }
        c.fadeToBlackBy(0);
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
