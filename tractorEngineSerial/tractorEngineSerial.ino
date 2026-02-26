#include <arduino.h>
// #include "Blink.h"
#include "Outputs.h"
#include "Pins.h"
#include "Debounce.h"
#include "Part.h"
// #include "Diagnostics.h"
#include <WDT.h>
// #include <Adafruit_AW9523.h>
#include "Audio.h"
#define DebounceMillis 100
#define BlinkMillis 400
// prompt the user with which part should be serviced
#define PromptMillis 15000
// turn off everything after this many prompts without a start
#define PromptCount 5

unsigned long promptTime = 0;
unsigned int promptCounter = 0;
Audio audio;
// Blink blink(BlinkMillis);
Outputs myOutputs;
// go button
Debounce goDebounce(StartButtonPin, DebounceMillis, true);
Part oilFilter(OilFilterPin, Outputs::OUTS::OilFilterLEDSR, myOutputs);
Part airFilter(AirFilterPin, Outputs::OUTS::AirFilterLEDSR, myOutputs);
Part dipStick(DipstickPin, Outputs::OUTS::DipstickLEDSR, myOutputs);
bool tractorOff = true;
// bool inDiagMode = false;

void setup() {
  Serial.begin(115200);
  unsigned long start = millis();
  const  unsigned long timeout = 2000;

  while (!Serial && (millis() - start < timeout))
    ;
  // SPI.begin();
  audio.begin();
  myOutputs.begin();
  goDebounce.begin();
  oilFilter.begin();
  // Serial.print(__LINE__); Serial.print(" ");Serial.println(myOutputs.sr_);
  airFilter.begin();
  //  Serial.print(__LINE__); Serial.print(" ");Serial.println(myOutputs.sr_);
  dipStick.begin();
  //  Serial.print(__LINE__); Serial.print(" ");Serial.println(myOutputs.sr_);
  myOutputs.setBitOff(Outputs::OUTS::StartButtonLEDSR);
  audio.play(Audio::Tracks::THANKYOU);
  Serial.println("running");
}

void tractorOn() {
  tractorOff = false;
  promptTime = millis();
  promptCounter = 0;
  // turn on the start button LED
  myOutputs.setBitOn(Outputs::OUTS::StartButtonLEDSR);
  oilFilter.goOn();
  airFilter.goOn();
  dipStick.goOn();
}

void turnTractorOff() {
  oilFilter.goOff();
  airFilter.goOff();
  dipStick.goOff();
  // turn off the start button LED
  myOutputs.setBitOff(Outputs::OUTS::StartButtonLEDSR);
  tractorOff = true;
}
// return true when ready to start.
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
  if (dipStick.getState() == Part::PartState::NoChange) {
    audio.play(Audio::Tracks::CheckOilLevel);
    return;
  }
  if (oilFilter.getState() == Part::PartState::NoChange) {
    audio.play(Audio::Tracks::CheckOilFilter);
    return;
  }
  if (airFilter.getState() == Part::PartState::NoChange) {
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
    Serial.println(__LINE__);
    if (tractorOff) {
      tractorOn();
      Serial.println(__LINE__);
    }
    if (statusCheck()) {
      audio.play(Audio::Tracks::EngineStarting);
      setPromptShutdown();
    } else {
      Serial.println(__LINE__);
      playAudio();
    }
  }
}

void serviceParts() {
  oilFilter.service();
  airFilter.service();
  dipStick.service();
}

// service the blinking LEDs
// void serviceBlink() {
//   Blink::TimerState ts = blink.service();
//   if (ts == Blink::TimerState::JustWentHigh || ts == Blink::TimerState::JustWentLow)
//     myOutputs.service(ts);
// }

// use the prompt timer to shut down the tracter after it starts
void setPromptShutdown() {
  promptTime = millis() + PromptMillis;
  promptCounter = PromptCount - 1;
}

// periodically prompt the user with the next part to check
void servicePrompt() {
  if (tractorOff)
    return;
  if (promptTime < millis()) {
    promptTime = promptTime + PromptMillis;
    promptCounter++;
    if (promptCounter >= PromptCount) {
      turnTractorOff();
    }
  }
}
void loop() {
  serviceGoButton();
  serviceParts();
  servicePrompt();
  myOutputs.service();
}

void loopx() {
  if (promptTime < millis()) {
    Serial.println(__LINE__);
    promptTime = promptTime + PromptMillis;
    myOutputs.setBitOn(Outputs::OUTS::StartButtonLEDSR);
    delay(500);
    myOutputs.setBitOff(Outputs::OUTS::StartButtonLEDSR);
  }
}
