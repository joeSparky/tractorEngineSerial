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

void Outputs::setDataToZeros() {
  sr_ = 0;
  transfer();
}

void Outputs::setBitOn(OUTS out) {
  sr_ |= (1 << static_cast<int>(out));
  transfer();
}

void Outputs::transfer() {
  SPI.transfer(sr_);
  digitalWrite(SRSelectPin, LOW);
  digitalWrite(SRSelectPin, HIGH);
}

void Outputs::transferFlashOn() {
  SPI.transfer(sr_ | blinkBits_);
  digitalWrite(SRSelectPin, LOW);
  digitalWrite(SRSelectPin, HIGH);
}

void Outputs::transferFlashOff() {
  SPI.transfer(sr_ & (~blinkBits_));
  digitalWrite(SRSelectPin, LOW);
  digitalWrite(SRSelectPin, HIGH);
}

void Outputs::setBitOff(OUTS out) {
  sr_ &= ~(1 << static_cast<int>(out));
  blinkBits_ &= ~(1 << static_cast<int>(out));
  transfer();
}

void Outputs::setBitFlash(OUTS out) {
  blinkBits_ |= (1 << static_cast<int>(out));
  sr_ |= (1 << static_cast<int>(out));
}

void Outputs::service() {
  Blink::TimerState ts = blink_.service();
  if (ts == Blink::TimerState::JustWentHigh) {
    transferFlashOn();
  }
  if (ts == Blink::TimerState::JustWentLow) {
    transferFlashOff();
  }
}
