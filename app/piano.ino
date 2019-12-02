int getKey() {
  int analogVal = analogRead(KEY_PIN);
  int button = 0;
  if (analogVal > 1000)
    button = 0;
  else if (analogVal < 100)
    button = 1;
  else if (analogVal < 150)
    button = 2;
  else if (analogVal < 250)
    button = 3;
  else if (analogVal < 300)
    button = 4;
  else if (analogVal < 400)
    button = 5;
  else if (analogVal < 450)
    button = 6;
  else if (analogVal < 500)
    button = 7;
  else if (analogVal < 550)
    button = 8;
    
  return button;
}


void playTone(int button) {
  if (button == 0) noTone(BUZZER_PIN);
  if (button == 1) tone(BUZZER_PIN, NOTE_C4);
  if (button == 2) tone(BUZZER_PIN, NOTE_D4);
  if (button == 3) tone(BUZZER_PIN, NOTE_E4);
  if (button == 4) tone(BUZZER_PIN, NOTE_F4);
  if (button == 5) tone(BUZZER_PIN, NOTE_G4);
  if (button == 6) tone(BUZZER_PIN, NOTE_A4);
  if (button == 7) tone(BUZZER_PIN, NOTE_B4);
  if (button == 8) tone(BUZZER_PIN, NOTE_C5);
}
