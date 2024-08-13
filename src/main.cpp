#include <FastLED.h>
#include <KeypadHandler.h>
#include <State.h>

#define LED_PIN     7
#define NUM_LEDS    100

auto keypad = KeypadHandler();

struct State ledState = {
  0, CRGB(255, 255, 255)
};

CRGB leds[NUM_LEDS];

bool processState() {
  Serial.println("update state");
  Serial.println(ledState.brightness);
  FastLED.setBrightness(ledState.brightness);
  
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ledState.color;
  }

  FastLED.show();
}

void setup() {
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);

  FastLED.show();
  Serial.begin(9600);
}

void loop() {
  auto changes = keypad.mutateState(ledState);

  if(changes == true) {
    processState();
  }
}