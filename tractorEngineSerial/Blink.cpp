#include "Blink.h"
// unsigned int delayAmount;
Blink::Blink(unsigned long d)
:
timer_(0),
state_(false),
delay_(d)
 {}

Blink::TimerState Blink::service() {
  if (millis() > timer_) {
    timer_ += delay_;
    state_ = !state_;
    // Serial.println("+");
    if (state_)
      return Blink::TimerState::JustWentHigh;
    else
      return Blink::TimerState::JustWentLow;
  }
  if (state_)
    return Blink::TimerState::IsHigh;
  else
    return Blink::TimerState::IsLow;
}