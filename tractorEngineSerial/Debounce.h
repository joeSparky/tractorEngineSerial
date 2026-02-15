#pragma once
#include <Arduino.h>
class Debounce {
  public:
  Debounce(uint8_t pin, int debounceMillis, bool activeLow=false);
  void begin();
  enum Event {
    None,
    Active,
    Inactive
  };
  Event service();
  bool getLastReading();
private:
bool activeLow_;
  uint8_t pin_;
  unsigned long lastTime_;
  // last digital read
  bool lastReading_;
  // what was last reported to the application
  bool lastReport_;
  // true until service() runs
  // bool firstTime_;
  int debounceMillis_;
};