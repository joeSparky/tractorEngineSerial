#pragma once
// #pragma message ">>> USING Outputs.h from tractor02 sketch folder <<<"
#include <Arduino.h>
#include <Adafruit_AW9523.h>
#include <Wire.h>
// #include <SPI.h>
// #include <SpiShiftRegisterChain.h>
#include "Pins.h"
// #include "Blink.h"
// constexpr uint8_t SRLatchPin = 10;
class Outputs {
public:
  // Outputs();
  void begin();
  // void resetAudio();
  enum class OUTS {
    StartButtonLEDSR,
    OilFilterLEDSR,
    DipstickLEDSR,
    AirFilterLEDSR,
     OutsCount
  };
  // void AudioResetOn();
  // void AudioResetOff();
  void setDataToZeros();
  // void writeData();
  void setBitOn(OUTS out);
  void setBitOff(OUTS out);
  // void onThenOff(OUTS out);
  // void setBlink(OUTS out);
  // void clearBlink(OUTS out);
  void service();
  void allOn();
  void allOff();
  // void allBlink();
private:
  Adafruit_AW9523 aw_;
  // Blink blink_;
  //bool blinkState_[static_cast<unsigned int>(Outputs::OUTS::OutsCount)];
};