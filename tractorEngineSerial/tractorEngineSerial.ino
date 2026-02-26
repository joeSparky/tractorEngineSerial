#include <arduino.h>
#include "WDT.h"
#include "Outputs.h"
#include "Pins.h"
#include "Debounce.h"
#include "Part.h"
// #include "Diagnostics.h"
#include <WDT.h>
// #include <Adafruit_AW9523.h>
#include "Audio.h"
// go button debounce time
#define DebounceMillis 100
// inactivity timer
#define InactiveMillis 60000
// engine starting audio time. Turn tractor off after playing
#define StartingMillis 10000
unsigned long activityTimer = 0;
Audio audio;
Outputs myOutputs;
// go button
Debounce goDebounce(StartButtonPin, DebounceMillis, true);
// parts
Part oilFilter(OilFilterPin, Outputs::OUTS::OilFilterLEDSR, myOutputs);
Part airFilter(AirFilterPin, Outputs::OUTS::AirFilterLEDSR, myOutputs);
Part dipStick(DipstickPin, Outputs::OUTS::DipstickLEDSR, myOutputs);
// tractor state
bool tractorOff = true;

void setup() {
  Serial.begin(115200);
  unsigned long start = millis();
  const unsigned long timeout = 2000;
  while (!Serial && (millis() - start < timeout))
    ;
  WDT.begin(5000);
  WDT.refresh();
  audio.begin();
  myOutputs.begin();
  goDebounce.begin();
  turnTractorOff();
  Serial.println("running");
}

void turnTractorOn() {
  tractorOff = false;
  activityTimer = millis();
  // turn on the start button LED
  myOutputs.setBitOn(Outputs::OUTS::StartButtonLEDSR);
  oilFilter.goOn();
  airFilter.goOn();
  dipStick.goOn();
  playAudio();
  resetActivityTimer();
}

void turnTractorOff() {
  oilFilter.begin();
  airFilter.begin();
  dipStick.begin();
  // turn off the start button LED
  myOutputs.setBitOff(Outputs::OUTS::StartButtonLEDSR);
  tractorOff = true;
  audio.play(Audio::Tracks::THANKYOU);
}
// return true when ready to start.
// otherwise, play the
// update the user on what needs to be done
void playAudio() {
  // encourage installation of missing parts
  if (dipStick.getState() == Part::PartState::PartRemoved) {
    audio.play(Audio::Tracks::InstallDipStick);
    return;
  }
  if (oilFilter.getState() == Part::PartState::PartRemoved) {
    audio.play(Audio::Tracks::InstallOilFilter);
    return;
  }
  if (airFilter.getState() == Part::PartState::PartRemoved) {
    audio.play(Audio::Tracks::InstallAirFilter);
    return;
  }

  // parts that haven't been serviced yet
  if (dipStick.getState() == Part::PartState::Initial) {
    audio.play(Audio::Tracks::CheckOilLevel);
    return;
  }
  if (oilFilter.getState() == Part::PartState::Initial) {
    audio.play(Audio::Tracks::CheckOilFilter);
    return;
  }
  if (airFilter.getState() == Part::PartState::Initial) {
    audio.play(Audio::Tracks::CheckAirFilter);
    return;
  }

  // everything's been serviced
  audio.play(Audio::Tracks::PressStart);
  return;
}

bool statusCheck() {
  if (tractorOff) return false;
  if (airFilter.getState() != Part::PartState::PartInstalled) return false;
  if (oilFilter.getState() != Part::PartState::PartInstalled) return false;
  if (dipStick.getState() != Part::PartState::PartInstalled) return false;
  return true;
}

void serviceGoButton() {
  if (goDebounce.service() == Debounce::Event::Active) {
    resetActivityTimer();
    if (tractorOff) {
      turnTractorOn();      
    }
    // if the engine is ready to start
    if (statusCheck()) {
      audio.play(Audio::Tracks::EngineStarting);
      setActivityTimer(StartingMillis);
    } else {
      // tell the user what needs to be done
      playAudio();
    }
  }
}

bool playedSomething(Part::PartState ps) {
  if (ps == Part::PartState::PartJustRemoved
      || ps == Part::PartState::PartJustInstalled) {
    playAudio();
    return true;
  } else {
    return false;
  }
}

void serviceParts() {
  if (tractorOff) return;
  if (playedSomething(oilFilter.service())) {
    resetActivityTimer();
    return;
  }
  if (playedSomething(airFilter.service())) {
    resetActivityTimer();
    return;
  }
  if (playedSomething(dipStick.service())) {
    resetActivityTimer();
    return;
  }
}

// use the prompt timer to shut down the tracter after the engine starting audio (about 10 seconds)
void resetActivityTimer() {
  activityTimer = millis() + InactiveMillis;
}

void setActivityTimer(unsigned long t){
  activityTimer = millis() + t;
}


void serviceActivityTimer() {
  if (tractorOff)
    return;
  if (activityTimer < millis())
    turnTractorOff();
}

void loop() {
  serviceGoButton();
  serviceParts();
  serviceActivityTimer();
  myOutputs.service();
  WDT.refresh();
}
