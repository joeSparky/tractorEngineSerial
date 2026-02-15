#include "Diagnostics.h"

Diagnostics::Diagnostics(Outputs &outputs, Blink &blink, Audio &audio)
  : outputs_(outputs),
    re_(2),
    display_(DClock, DData),
    // selectSwitch_(SelectorPin, 100),
    blink_(blink),
    audio_(audio) {
  // state_ = Diagnostics::MyState::Run;
  // lastExecuted_ =
  state_ = MyState::Run;
}

void Diagnostics::begin() {
  display_.setBrightness(0x0f);
  display_.clear();
  display_.showNumberDec(state_);
  pinMode(DiagnosticsLED, OUTPUT);
  pinMode(APin, INPUT);
  pinMode(BPin, INPUT);
  SPI.begin();
  digitalWrite(DiagnosticsLED, HIGH);
}

void Diagnostics::leaving(Diagnostics::MyState state) {
  // turn everything off
  outputs_.setDataToZeros();
  // outputs_.writeData();

  // resume audio
  // outputs_.AudioResetOff();

  if (state == Diagnostics::MyState::DiagLEDState)
    digitalWrite(DiagnosticsLED, HIGH);
}
void Diagnostics::entering(Diagnostics::MyState state) {
  // turn everything off
  outputs_.setDataToZeros();
  switch (state) {
    case Diagnostics::MyState::AllOutputsBlinkState:
      // outputs_.AudioResetOn();
      break;
    case Diagnostics::MyState::AllOutputsOnState:
      outputs_.allOn();
      break;
    case Diagnostics::MyState::AllOutputsOffState:
      outputs_.allOff();
      break;
    case Diagnostics::MyState::ThankYou:
      audio_.play(Audio::Tracks::THANKYOU);
      break;
    case Diagnostics::MyState::CheckOilLevel:
      audio_.play(Audio::Tracks::CheckOilLevel);
      break;
    case Diagnostics::MyState::CheckAirFilter:
      audio_.play(Audio::Tracks::CheckAirFilter);
      break;
    case Diagnostics::MyState::CheckOilFilter:
      audio_.play(Audio::Tracks::CheckOilFilter);
      break;
    case Diagnostics::MyState::OilFilterMissing:
      audio_.play(Audio::Tracks::InstallOilFilter);
      break;
    case Diagnostics::MyState::AirFilterMissing:
      audio_.play(Audio::Tracks::InstallAirFilter);
      break;
    case Diagnostics::MyState::InstallDipStick:
      audio_.play(Audio::Tracks::InstallDipStick);
      break;
    case Diagnostics::MyState::PressStartButton:
      audio_.play(Audio::Tracks::PressStart);
      break;
    case Diagnostics::MyState::EngineStarting:
      audio_.play(Audio::Tracks::EngineStarting);
      break;
  }
}


void Diagnostics::running() {
  Blink::TimerState myBlink = blink_.service();
  switch (state_) {
    case Diagnostics::MyState::DiagLEDState:
      digitalWrite(DiagnosticsLED, myBlink == Blink::TimerState::IsHigh);
      break;
    case Diagnostics::MyState::AllOutputsBlinkState:
      if (myBlink == Blink::TimerState::JustWentHigh)
        outputs_.allOn();
      if (myBlink == Blink::TimerState::JustWentLow)
        outputs_.allOff();
      break;
    case Diagnostics::MyState::StartButtonState:
      setDiagLED(digitalRead(StartButtonPin));
      break;
    case Diagnostics::MyState::DipStickSwitchState:
      setDiagLED(digitalRead(DipstickPin));
      break;
    case Diagnostics::MyState::OilFiliterSwitchState:
      setDiagLED(digitalRead(OilFilterPin));
      break;
    case Diagnostics::MyState::AirFilterSwitchState:
      setDiagLED(digitalRead(AirFilterPin));
      break;
    case Diagnostics::MyState::AudioActive:
      setDiagLED(digitalRead(AudioActivePin));
    default:
      break;
  }
}

// copy the switch level to both the on-board LED and the go button
// so that we can diagnose switches from the outside of the tractor
void Diagnostics::setDiagLED(bool level) {
  digitalWrite(DiagnosticsLED, level);
  if (level)
    outputs_.setBitOn(Outputs::OUTS::StartButtonLEDSR);
  else
    outputs_.setBitOff(Outputs::OUTS::StartButtonLEDSR);
}

void Diagnostics::serviceRotaryEncoder() {
  // let the rotary encoder determine what runs
  switch (re_.update(digitalRead(APin), digitalRead(BPin))) {
    case RotaryEncoder::Increase:
      leaving(state_);
      state_ = static_cast<MyState>((state_ + 1) % MyState::StateCount);
      entering(state_);
      display_.showNumberDec(state_);
      break;
    case RotaryEncoder::Decrease:
      leaving(state_);
      state_ = static_cast<MyState>(static_cast<int>(state_) - 1);
      if (state_ < 0)
        state_ = static_cast<MyState>(MyState::StateCount - 1);
      entering(state_);
      display_.showNumberDec(state_);
      break;
    case RotaryEncoder::None:
      break;
    default:
      break;
  }
}

Diagnostics::MyState Diagnostics::service() {
  serviceRotaryEncoder();
  // serviceBlink();
  running();
  return Diagnostics::MyState::Run;
}