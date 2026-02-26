#pragma once
#include "Outputs.h"
#include "Debounce.h"
// make part debounce time very long so that kids aren't
// bugged by audio while struggling to install a part
#define PartDebounceMillis 1000

class Outputs;

class Part {
public:
  Part(uint8_t pin, Outputs::OUTS partLed, Outputs& outputs);

  enum PartState {
    Initial,
    PartRemoved,
    PartInstalled,
    PartJustRemoved,
    PartJustInstalled
  };

  PartState getState();
  PartState service();
  void begin();
  void goOn();
  void goOff();
  // private:
  PartState partState_;
  Outputs::OUTS partLED_;

  // uint8_t pin_;
  Debounce debounce_;
  // bool firstTime_;
private:
  Outputs& outputs_;
  // Blink blink_;
};