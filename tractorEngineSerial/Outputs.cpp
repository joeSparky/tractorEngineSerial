#include "Outputs.h"
// Outputs::Outputs()
//   : aw_(SRLatchPin, 2){};

void Outputs::begin() {
  Outputs::setDataToZeros();
}
void Outputs::setDataToZeros() {
  for (int i=0; i<static_cast<int>(Outputs::OUTS::OutsCount); i++)
    aw_.digitalWrite(i, LOW);
}

// void Outputs::resetAudio() {
//   digitalWrite(AudioResetPin, LOW);
//   pinMode(AudioResetPin, OUTPUT);
//   delay(100);
//   digitalWrite(AudioResetPin, HIGH);
//   pinMode(AudioActivePin, INPUT);
// }

// void Outputs::writeData() {
//   aw_.writeData();
// }

void Outputs::setBitOn(OUTS out) {
  aw_.digitalWrite(static_cast<int>(out), HIGH);
  // Serial.print(__LINE__);Serial.print(":");Serial.println(static_cast<uint8_t>(out));
}

void Outputs::setBitOff(OUTS out) {
  aw_.digitalWrite(static_cast<int>(out), LOW);
  // Serial.println(__LINE__);
}

// void Outputs::onThenOff(OUTS out) {
//   // ignore request if there's arlready something playing
//   if (!digitalRead(AudioActivePin)) {
//     return;
//   }
//   aw_.setBitOn(static_cast<int>(out));
//   while (digitalRead(AudioActivePin)) {
//     delay(100);
//     // Serial.print(__LINE__);
//     // Serial.print(":");
//     // Serial.println(static_cast<int>(out));
//   }
//   aw_.setBitOff(static_cast<int>(out));
// }

void Outputs::allOn() {
  for (unsigned int i = 0; i < static_cast<unsigned int>(Outputs::OUTS::OutsCount); i++) {
    aw_.digitalWrite(i, HIGH);
  }
  // Serial.println(__LINE__);
}
void Outputs::allOff() {
  Outputs::setDataToZeros();
  // aw_.writeData();
  // Serial.println(__LINE__);
}

// void Outputs::AudioResetOn() {
//   digitalWrite(AudioResetPin, LOW);
// }
// void Outputs::AudioResetOff() {
//   digitalWrite(AudioResetPin, HIGH);
// }
