#include "Debounce.h"
#include <Arduino.h>
Debounce::Debounce(uint8_t pin, int debounceMillis, bool activeLow)
  : activeLow_(activeLow) {
  pin_ = pin;
  debounceMillis_ = debounceMillis;
  lastTime_ = 0;
  lastReading_ = false;
}

void Debounce::begin() {
  pinMode(pin_, INPUT);
  lastReading_ = lastReport_ = digitalRead(pin_);
}
Debounce::Event Debounce::service() {
  bool tmp = digitalRead(pin_);
  // if the input has changed since the last time service() was called
  if (tmp != lastReading_) {
    // remember the last internal reading
    lastReading_ = tmp;
    // restart the timer
    lastTime_ = millis() + debounceMillis_;
  }
  // if the debounce timer has expired
  if (lastTime_ < millis()) {
    // if the internal state matches what was last reported
    if (lastReport_ == lastReading_) {
      return Event::None;
    } else {
      // have a new switch event to report
      lastReport_ = lastReading_ = tmp;
      return tmp ^ activeLow_ ? Event::Active : Event::Inactive;
    }
  }
  return Event::None;
}

bool Debounce::getLastReading() {
  return lastReading_;
}
