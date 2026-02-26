#include "Part.h"

Part::Part(uint8_t pin, Outputs::OUTS partLED, Outputs& outputs)
  : partLED_(partLED),
    debounce_(pin, PartDebounceMillis, true),
    outputs_(outputs)
{
  // Part::goOff();
};

void Part::begin() {
  goOff();
}

void Part::goOff() {
  outputs_.setBitOff(partLED_);
  partState_ = Part::PartState::Initial;
}

void Part::goOn() {
  outputs_.setBitFlash(partLED_);
}

Part::PartState Part::service() {
  Debounce::Event event = debounce_.service();
  // manage the switch
  switch (event) {
    case Debounce::Event::None:
      break;
      // the magnet was pulled away from the reed switch,
      // the reed switch opens, the voltage goes up
    case Debounce::Event::Inactive:
      partState_ = Part::PartState::PartRemoved;
      // turn on so we can flash it
      outputs_.setBitFlash(partLED_);
      return Part::PartState::PartJustRemoved;
      // part put back on the tractor
    case Debounce::Event::Active:
      partState_ = Part::PartState::PartInstalled;
      outputs_.setBitOn(partLED_);
      return Part::PartState::PartJustRemoved;
  }
  return partState_;
}

Part::PartState Part::getState(){
  return partState_;
}