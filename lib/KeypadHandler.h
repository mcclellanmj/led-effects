#pragma once
// KeypadHandler.h

#include <Keypad.h>
#include <FastLED.h>
#include "State.h"

enum Modifier { NONE, BRIGHTNESS, COLOR };
CRGB colors[] = { CRGB (255, 255, 255), CRGB(252,137,35), CRGB( 119, 0, 184) };

const byte ROWS = 4;
const byte COLS = 4;

class KeypadHandler {
public:
    KeypadHandler();
    bool mutateState(State &state);

private:
    bool processKey(char &key, State &state);
    bool processMultiKey(char &key, State &state);
    bool processBrightness(char &key, State &state);
    bool processColor(char &key, State &state);

    Keypad keypad;
    char keys[ROWS][COLS] = {
        {'1','2','3','A'},
        {'4','5','6','B'},
        {'7','8','9','C'},
        {'*','0','#','D'}
    };
    byte rowPins[ROWS] = {13, 12, 11, 10};
    byte colPins[COLS] = {2, 3, 4, 5};

    Modifier inputModifier = NONE;
};

KeypadHandler::KeypadHandler() 
    : keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS) {}

bool KeypadHandler::processKey(char &key, State &state) {
  if(key == 'B') {
    this->inputModifier = BRIGHTNESS;
    return false;
  } else if(key == 'C') {
    this->inputModifier = COLOR;
    return false;
  } else if(key == '#') {
    this->inputModifier = NONE;
    return false;
  } else {
    return processMultiKey(key, state);
  }
}

bool KeypadHandler::processMultiKey(char &key, State &state) {
  if(this->inputModifier == BRIGHTNESS) {
    return processBrightness(key, state);
  } else if(this->inputModifier == COLOR) {
    return this->processColor(key, state);
  } else {
    return false;
  }
}

bool KeypadHandler::processColor(char &c, State &state) {
  if(isdigit(c)) {
    uint8_t index = c - '0';

    uint8_t array_length = sizeof(colors) / sizeof(colors[0]);
    Serial.println(array_length);
    if(index < array_length + 1) {
      Serial.println("Setting color to");
      Serial.println(index);
      state.color = colors[c - '0'];
      return true;
    }
  }
  return false;
}

bool KeypadHandler::processBrightness(char &c, State &state) {
  if(isdigit(c)) {
    if(c == '0') {
      state.brightness = 0;
    } else if (c == '1') { 
      state.brightness = 5;
    } else {
      uint8_t amt = (c - '0') + 1;
      state.brightness = amt * 25.5;
    }
    return true;
  }
  return false;
}

bool KeypadHandler::mutateState(State &state) {
    char key = keypad.getKey();
    if (key != '\0') {
        return this->processKey(key, state);
    } else {
        return false;
    }
}