#include "Outputs.h"
Outputs::Outputs()
  : blink_(BlinkMillis) {
  sr_ = 0;
  blinkBits_ = 0;
};

void Outputs::begin() {
  SPI.begin();
  SPI.beginTransaction(SPISettings());
  pinMode(SRSelectPin, OUTPUT);
  digitalWrite(SRSelectPin, HIGH);
  sr_ = 0;
  blinkBits_ = 0;
  transfer();
}

void Outputs::setBitOn(OUTS out) {
  uint8_t mask = 1 << static_cast<int>(out);
  sr_ |= mask;
  blinkBits_ &= ~mask;
  transfer();
}

void Outputs::transfer() {
  SPI.transfer(sr_);
  digitalWrite(SRSelectPin, LOW);
  digitalWrite(SRSelectPin, HIGH);
}

// first half of a cycle
// if blinkBit_ is off, display the data.
// if blinkBit_ is on, turn on the displayed bit
void Outputs::transferPhase1() {
  SPI.transfer(sr_ | blinkBits_);
  digitalWrite(SRSelectPin, LOW);
  digitalWrite(SRSelectPin, HIGH);
}

// second half of a cycle
// if blinkBit_ is off, display the data
// if blinkBit_ is on, turn off the display bit
void Outputs::transferPhase2() {
  SPI.transfer(sr_ & (~blinkBits_));
  digitalWrite(SRSelectPin, LOW);
  digitalWrite(SRSelectPin, HIGH);
}

void Outputs::setBitOff(OUTS out) {
  uint8_t mask = 1 << static_cast<int>(out);
  sr_ &= ~mask;
  blinkBits_ &= ~mask;
  transfer();
}

void Outputs::setBitFlash(OUTS out) {
  uint8_t mask = 1 << static_cast<int>(out);  
  blinkBits_ |= mask;
}

void Outputs::service() {
  Blink::TimerState ts = blink_.service();
  if (ts == Blink::TimerState::JustWentHigh) {
    transferPhase1();
  }
  if (ts == Blink::TimerState::JustWentLow) {
    transferPhase2();
  }
}
