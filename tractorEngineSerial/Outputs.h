#pragma once
// #pragma message ">>> USING Outputs.h from tractor02 sketch folder <<<"
#include <Arduino.h>
// #include <bitset>
#include <SPI.h>
// #include <SpiShiftRegisterChain.h>
#include "Pins.h"
#include "Blink.h"
#define BlinkMillis 800
// constexpr uint8_t SRLatchPin = 10;
class Outputs {
public:
  Outputs();
  void begin();
  // void resetAudio();
  enum class OUTS {
    StartButtonLEDSR,
    OilFilterLEDSR,
    DipstickLEDSR,
    AirFilterLEDSR,
     OutsCount
  };
  void setBitOn(OUTS out);
  void setBitOff(OUTS out);
  void setBitFlash(OUTS out);
  // call this to make the outputs blink
  void service();
private:
  // display data. blinking action defined by blinkBits_
  uint8_t sr_;
  Blink blink_;
  void transfer();  
  void transferPhase1();
  void transferPhase2();
 // which outputs should blink
  uint8_t blinkBits_;
};