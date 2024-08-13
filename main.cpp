#include <FastLED.h>
#include <Keypad.h>
#include <EEPROM.h>
#define LED_PIN     7
#define NUM_LEDS    100

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {13, 12, 11, 10}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {2, 3, 4, 5}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

enum Modifier { NONE, BRIGHTNESS, COLOR };

struct State {
  uint8_t brightness;
  Modifier inputModifier;
  CRGB color;
};

struct State ledState = {
  0, NONE, CRGB(255, 255, 255)
};

CRGB leds[NUM_LEDS];

CRGB colors[] = { CRGB (255, 255, 255), CRGB(252,137,35), CRGB( 119, 0, 184) };

void setup() {
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);

  FastLED.show();
  Serial.begin(9600);
}

void loop() {
  char key = keypad.getKey();

  if(key != '\0') {
    bool update = processKey(key);
    if (update) {
      processState();
    }
  }
}

bool processState() {
  Serial.println("update state");
  Serial.println(ledState.brightness);
  FastLED.setBrightness(ledState.brightness);
  
  for (int i = 0; i <= NUM_LEDS; i++) {
    leds[i] = ledState.color;
  }

  FastLED.show();
}

bool processKey(char key) {
  if(key == 'B') {
    ledState.inputModifier = BRIGHTNESS;
    return false;
  } else if(key == 'C') {
    ledState.inputModifier = COLOR;
    return false;
  } else if(key == '#') {
    ledState.inputModifier = NONE;
    return false;
  } else {
    return processMultiKey(key);
  }
}

bool processMultiKey(char key) {
  if(ledState.inputModifier == BRIGHTNESS) {
    return processBrightness(key);
  } else if(ledState.inputModifier == COLOR) {
    return processColor(key);
  }
}

bool processColor(char c) {
  if(isdigit(c)) {
    uint8_t index = c - '0';

    uint8_t array_length = sizeof(colors) / sizeof(colors[0]);
    Serial.println(array_length);
    if(index < array_length + 1) {
      Serial.println("Setting color to");
      Serial.println(index);
      ledState.color = colors[c - '0'];
      return true;
    }
  }
  return false;
}

bool processBrightness(char c) {
  if(isdigit(c)) {
    if(c == '0') {
      ledState.brightness = 0;
    } else if (c == '1') { 
      ledState.brightness = 10;
    } else {
      uint8_t amt = (c - '0') + 1;
      ledState.brightness = amt * 25.5;
    }
    return true;
  }
  return false;
}