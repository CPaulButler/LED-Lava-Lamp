#ifndef randomdefs
#define randomdefs

  #include <FastLED.h>
  #include "global.h"

  void DrawRandom() {
    static int x = random(kMatrixWidth);
    static int y = random(kMatrixHeight);
    static CRGB c = CRGB(0, 0, 0);
    blur2d(g_LEDs, kMatrixWidth, kMatrixHeight, 100, xymap);
    EVERY_N_MILLISECONDS(100) {
      x = random(kMatrixWidth);
      y = random(kMatrixHeight);
      uint8_t r = random(256);
      uint8_t g = random(256);
      uint8_t b = random(256);
      c = CRGB(r, g, b);
    }
    g_LEDs[xymap(x, y)] = c;

    FastLED.show();
    delay(20);
  }

#endif
