#include "Audio.h"

Audio::Audio()
  : sfx_(&Serial1, NULL, AudioResetPin) {
}
void Audio::play(Tracks track) {
  size_t idx = static_cast<size_t>(track);

  char name[13];                 // 12 chars + NUL (per Adafruit example comment)
  strncpy(name, trackNames[idx], sizeof(name) - 1);
  name[sizeof(name) - 1] = '\0';

  sfx_.playTrack(name);
  Serial.print("playing:");Serial.println(name);
}

void Audio::begin() {
  Serial1.begin(9600);
  while (!Serial1)
    ;
  if (!sfx_.reset())
    Serial.println("fx not found");
  // Audio::play(Tracks::THANKYOU);
}