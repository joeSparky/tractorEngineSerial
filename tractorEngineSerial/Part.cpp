#include "Part.h"

Part::Part(uint8_t pin, Outputs::OUTS partLED, Outputs& outputs, Blink& blink)
  : partLED_(partLED),
    debounce_(pin, 100, true),
    // firstTime_(true),
    outputs_(outputs),
    blink_(blink) {
  partState_ = Part::PartState::Off;
};
Part::PartState Part::getState() {
  return partState_;
};

void Part::begin() {
  goOn();
}

void Part::goOff() {
  partState_ = Part::PartState::Off;
  outputs_.setBitOff(partLED_);
}

void Part::goOn() {
  // if the part is on the tractor
  // switch is normally open, the magnet in the part closes the switch
  // high reading with the part missing
  if (debounce_.getLastReading())
    partState_ = Part::PartState::Missing;
  else
    // the part is on the tractor but needs to be removed
    partState_ = Part::PartState::NeedsServicing;
  outputs_.setBitOff(partLED_);
}

void Part::service() {
  if (partState_ == Part::PartState::Off)
  return;
  /////////////////////////////////
  // manage the switch
  switch (debounce_.service()) {
    case Debounce::Event::None:
      break;
      // the magnet was pulled away from the reed switch,
      // the reed switch opens, the voltage goes up
    case Debounce::Event::Inactive:
      partState_ = Part::PartState::Missing;
      // turn on so we can flash it
      outputs_.setBitOn(partLED_);
      break;
      // part put back on the tractor
    case Debounce::Event::Active:
      partState_ = Part::PartState::Serviced;
      outputs_.setBitOn(partLED_);
      break;
  }

  ///////////////////////////////
  // manage the LED
  switch (blink_.service()) {
      // flash if missing. otherwise, no change
    case (Blink::TimerState::JustWentHigh):
      if ((partState_ == Part::PartState::Missing) || (partState_ == Part::PartState::NeedsServicing))
        outputs_.setBitOn(partLED_);
      break;

      // blink when the part is missing, turn off when serviced
    case (Blink::TimerState::JustWentLow):
      if ((partState_ == Part::PartState::Missing) || (partState_ == Part::PartState::Serviced))
        outputs_.setBitOff(partLED_);
      break;
    default:
      break;
  }
}