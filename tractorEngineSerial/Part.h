#pragma once
#include "Outputs.h"
#include "Debounce.h"
// #include "Blink.h"

class Outputs;

class Part {
public:
  Part(uint8_t pin, Outputs::OUTS partLed, Outputs& outputs);

  enum PartState {
    NoChange,
    PartRemoved,
    PartInstalled
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