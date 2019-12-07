#include "config.h"

int getKey() {
  int analogVal = analogRead(A0);
  int button = 0;
  if (analogVal >= 550)
    button = 0;
  else if (analogVal < 50)
    button = 1;
  else if (analogVal < 132)
    button = 2;
  else if (analogVal < 200)
    button = 3;
  else if (analogVal < 240)
    button = 4;
  else if (analogVal < 320)
    button = 5;
  else if (analogVal < 400)
    button = 6;
  else if (analogVal < 450)
    button = 7;
  else if (analogVal < 550)
    button = 8;
    
  return button;
}


void playTone(int button) {
  if (button == 0) noTone(BUZZER_PIN);
  if (button == 1) tone(BUZZER_PIN, NOTE_C6);
  if (button == 2) tone(BUZZER_PIN, NOTE_D6);
  if (button == 3) tone(BUZZER_PIN, NOTE_E6);
  if (button == 4) tone(BUZZER_PIN, NOTE_F6);
  if (button == 5) tone(BUZZER_PIN, NOTE_G6);
  if (button == 6) tone(BUZZER_PIN, NOTE_A6);
  if (button == 7) tone(BUZZER_PIN, NOTE_B6);
  if (button == 8) tone(BUZZER_PIN, NOTE_C7);
}
