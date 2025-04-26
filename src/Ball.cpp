#include <algorithm>

#define HEIGHT (NUM_LEDS/4)
#define BALLSIZE (HEIGHT / 4)

CRGB frontLEDs[HEIGHT] = {0};
CRGB backLEDs[HEIGHT] = {0};
CRGB leftLEDs[HEIGHT] = {0};
CRGB rightLEDs[HEIGHT] = {0};


void DrawBall() {
  static byte hue = HUE_RED;
  const int deltaHue = 3;

  static float x = 0;
  static float dx = (float) HEIGHT / 100.0;

  x = x + dx;

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
