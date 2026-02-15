#pragma once
#include <Arduino.h>
class Blink {
public:
  Blink(unsigned long delay);
  enum TimerState {
    JustWentHigh,
    JustWentLow,
    IsHigh,
    IsLow
  };

  TimerState service();
private:
  unsigned long timer_;
  bool state_;
  unsigned long delay_;
};