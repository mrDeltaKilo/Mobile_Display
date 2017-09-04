/********************
* Indicator_test.h *
********************/

#if defined(BOARD_LED_PIN_WS2812)
  #include <FastLED.h>
  #define NUM_LEDS          1
  #define LED_TYPE          WS2812B
  #define COLOR_ORDER       GRB
CRGB leds[NUM_LEDS];
  #define FRAMES_PER_SECOND 120
  #if !defined(BOARD_RGB_BRIGHTNESS)
    #define BOARD_RGB_BRIGHTNESS  96
  #endif
FastLED.setBrightness(BOARD_RGB_BRIGHTNESS);
#endif

class Indicator {
public:

protected:
#if defined(BOARD_LED_PIN_WS2812) || defined(BOARD_LED_PIN_R)
  #if defined(BOARD_LED_PIN_WS2812)
    void initLED() {
      FastLED.addLeds<LED_TYPE, BOARD_LED_PIN_WS2812, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

    }
    void setRGB(uint32_t color) {
      leds[0] = color;
      FastLED.show();
    }
private:


};
