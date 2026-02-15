#pragma once
#include "Outputs.h"
#include "Debounce.h"
#include "Blink.h"

class Outputs;

class Part {
public:
  Part(uint8_t pin, Outputs::OUTS partLed, Outputs& outputs, Blink& blink);

  enum PartState {
    // on reset, if the part is on the tractor, go to this state
    // LED on
    NeedsServicing,
    // on reset, if the part is not on the tractor, go to this state
    // LED flashing
    Missing,
    // if the part has been taken off and then put back on, go to this state
    // LED off
    Serviced,
// the tractor has completed a cycle and is waiting for the child to press the start button.
// LED stays off
    Off
  };

  PartState getState();
  void service();
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
  Outputs outputs_;
  Blink blink_;
};