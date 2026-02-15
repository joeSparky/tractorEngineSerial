#pragma once
#include "Adafruit_Soundboard.h"
#include "Pins.h"
class Audio {
public:
  Audio();
  enum class Tracks {
    CheckOilLevel,
    CheckAirFilter,
    CheckOilFilter,
    InstallOilFilter,
    InstallAirFilter,
    InstallDipStick,
    PressStart,
    EngineStarting,
    THANKYOU,
    SOUND_COUNT
  };
  const char* trackNames[static_cast<int>(Tracks::SOUND_COUNT)] = {
    "LEVCHK  OGG",
    "AIRCHK  OGG",
    "OILCHK  OGG",
    "OILINS  OGG",
    "AIRINS  OGG",
    "DIPINS  OGG",
    "PUSHGRN OGG",
    "STARTINGOGG",
    "THANKS  OGG"
  };

  void play(Tracks track);
  void begin();
private:
  Adafruit_Soundboard sfx_;
};