#include <ws2812.h>
#define NUM_LEDS 1
#define DATA_PIN 18

CRGB leds[NUM_LEDS];

void setup_ws2812() {
    FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
}

void ws2812_show(const int color) {
    leds[0] = color;
    FastLED.show();
}
