#pragma once
#include <Arduino.h>
#include "RotaryEncoder.h"
#include "TM1637Display.h"
#include "Blink.h"
#include "Debounce.h"
#include "Pins.h"
#include "Outputs.h"
#include "Audio.h"


#define DiagnosticsLED A0
// #define BlinkMillis 250
class Diagnostics {
public:
  Diagnostics(Outputs& outputs, Blink& blink, Audio& audio);
  Outputs& outputs_;
  Blink& blink_;
  void begin();
  enum MyState {
    Run,
    // outputs
    DiagLEDState,
    AllOutputsOnState,
    AllOutputsOffState,
    AllOutputsBlinkState,
    // audio triggers
    ThankYou,
    CheckOilLevel,
    CheckAirFilter,
    CheckOilFilter,
    OilFilterMissing,
    AirFilterMissing,
    InstallDipStick,
    PressStartButton,
    EngineStarting,
    // inputs
    StartButtonState,
    DipStickSwitchState,
    OilFiliterSwitchState,
    AirFilterSwitchState,
    AudioActive,
    // other
    StateCount
  };
  // MyState nextState_;
  void leaving(Diagnostics::MyState state);
  void entering(Diagnostics::MyState state);
  MyState service();
  void running();
  void serviceBlink();
  MyState state_;
  RotaryEncoder re_;
  TM1637Display display_;
private:
  void serviceRotaryEncoder();
  void setDiagLED(bool level);
  Audio audio_;
};