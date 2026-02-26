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
  void service(Blink::TimerState ts);
  // void AudioResetOff();
  void setDataToZeros();
  // void writeData();
  void setBitOn(OUTS out);
  void setBitOff(OUTS out);
  void setBitFlash(OUTS out);
  void service();
  void allOn();
  void allOff();
  // void allBlink();
  uint8_t sr_;
  Blink blink_;
private:
  void transfer();
  void transferFlash();
  void transferFlashOn();
  void transferFlashOff();
 // shift register contents
  
  // which outputs should blink
  uint8_t blinkBits_;
  //bool blinkState_[static_cast<unsigned int>(Outputs::OUTS::OutsCount)];
};