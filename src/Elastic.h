#ifndef elasticdefs
#define elasticdefs

  #include "global.h"
  float sign(float x) {
    if (x < 0.0) return -1.0;
    return 1.0;
  }

  #define SPRING_LENGTH 20.0
  #define SPRING_CONST 0.10
  // compute Force on 'me'. Positive pushes to larger position, Negative pulls toward 0.
  float ForceMe(float me, float they) {
    return - sign(they - me) * SPRING_CONST * (SPRING_LENGTH - abs(me - they));
  }

  float min(float x, float y) {
    if (x < y) return x;
    return y;
  }

  float max(float x, float y) {
    if (x > y) return x;
    return y;
  }

  // truncate to valid range of LEDs
  float Normalize(float x) {
    if (x < 0.0) return -x;
    if (x >= NUM_LEDS) return NUM_LEDS - 1.0 - (x - NUM_LEDS);
    return x;
  }

  float Speed(float dx, float mass, float force) {
    return dx + float(force)/mass;
  }

  bool OutOfBounds(float x) {
    return (x < 0.0) || (x >= NUM_LEDS);
  }

  void DrawElastic() {
    static byte hue = HUE_RED;
    const int deltaHue = 3;

    static float x1=SPRING_LENGTH + 0.40 * SPRING_LENGTH;
    static float x2=0.0;
    static float dx1=0.3;
    static float dx2=0.0;
    const float mass1=150.0;
    const float mass2=150.0;

    x2 = x2 + dx2;
    x1 = x1 + dx1;

    dx2 = Speed(dx2, mass2, ForceMe(x2, x1));
    if (OutOfBounds(x2)) dx2 = -dx2;
    dx1 = Speed(dx1, mass1, ForceMe(x1, x2));
    if (OutOfBounds(x1)) dx1 = -dx1;

    x2 = Normalize(x2);
    x1 = Normalize(x1);


      // Randomly fade the LEDs
      for (int j = 0; j < NUM_LEDS; j++)
        g_LEDs[j] = g_LEDs[j].fadeToBlackBy(127);

    hue += deltaHue;
    for (int i = int(min(x1,x2)); i <= int(max(x1,x2)); i++)
      g_LEDs[i].setHue(hue);

    g_LEDs[int(x1)] = CRGB::Red;
    g_LEDs[int(x2)] = CRGB::Green;
    FastLED.show();
    delay(20);
  }

#endif
