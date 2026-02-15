#include <arduino.h>
#include "Blink.h"
#include "Outputs.h"
#include "Pins.h"
#include "Debounce.h"
#include "Part.h"
#include "Diagnostics.h"
#include <WDT.h>
// #include <Adafruit_AW9523.h>
#include "Audio.h"
#define DebounceMillis 100
#define BlinkMillis 400

Audio audio;
// Adafruit_AW9523 aw;
Blink blink(BlinkMillis);
Outputs myOutputs;
Diagnostics diag(myOutputs, blink, audio);
Debounce goDebounce(StartButtonPin, DebounceMillis, true);
Part oilFilter(OilFilterPin, Outputs::OUTS::OilFilterLEDSR, myOutputs, blink);
Part airFilter(AirFilterPin, Outputs::OUTS::AirFilterLEDSR, myOutputs, blink);
Part dipStick(DipstickPin, Outputs::OUTS::DipstickLEDSR, myOutputs, blink);
bool tractorOff = true;

void setup() {
  // save the watchdog timer status
  // uint8_t mcusr = MCUSR;
  // clear the watchdog timer status
  // MCUSR = 0;
  
  // wdt_disable();
  Serial.begin(115200);
  while (!Serial);
  Serial.println("running");
  audio.begin();
  // aw.begin(0x58);
  myOutputs.begin();
  diag.begin();
  goDebounce.begin();
  oilFilter.goOff();
  airFilter.goOff();
  dipStick.goOff();
  myOutputs.setBitOff(Outputs::OUTS::StartButtonLEDSR);
  Serial.println("running");
  // if we just went through a watchdog timer reset
  // if (mcusr & _BV(WDRF))
  //   Serial.println("wdt reset");
  // wdt_enable(WDTO_8S);
}

void serviceButtons() {
  if (diag.state_ != Diagnostics::MyState::Run)
    return;
  // if the start button has been pressed
  if (goDebounce.service() == Debounce::Event::Active) {
    // Serial.println(__LINE__);
    if (tractorOff) {
      tractorOff = false;
      // turn on the start button LED
      myOutputs.setBitOn(Outputs::OUTS::StartButtonLEDSR);
      oilFilter.goOn();
      airFilter.goOn();
      dipStick.goOn();
    }
    ////////////////////////////////////////////
    // dipstick
    Part::PartState state = dipStick.getState();
    if (state == Part::PartState::Missing) {
      audio.play(Audio::Tracks::InstallDipStick);
      return;
    }
    if (state == Part::PartState::NeedsServicing) {
      audio.play(Audio::Tracks::CheckOilLevel);
      return;
    }

    ///////////////////////////////////////////
    // air filter
    state = airFilter.getState();
    if (state == Part::PartState::Missing) {
      audio.play(Audio::Tracks::InstallAirFilter);
      return;
    }
    if (state == Part::PartState::NeedsServicing) {
      audio.play(Audio::Tracks::CheckAirFilter);
      return;
    }

    ///////////////////////////////////////////
    // oil filter
    state = oilFilter.getState();
    if (state == Part::PartState::Missing) {
      audio.play(Audio::Tracks::InstallOilFilter);
      return;
    }
    if (state == Part::PartState::NeedsServicing) {
      audio.play(Audio::Tracks::CheckOilFilter);
      return;
    }


    audio.play(Audio::Tracks::EngineStarting);
    oilFilter.goOff();
    airFilter.goOff();
    dipStick.goOff();
    // turn off the start button LED
    myOutputs.setBitOff(Outputs::OUTS::StartButtonLEDSR);
    tractorOff = true;
  }
}


void serviceParts() {
  if (diag.state_ != Diagnostics::MyState::Run)
    return;
  oilFilter.service();
  airFilter.service();
  dipStick.service();
}

void loop() {
  // wdt_reset();
  diag.service();
  serviceButtons();
  serviceParts();
}
