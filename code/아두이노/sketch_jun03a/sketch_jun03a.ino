int pins[]={2,3,4};
int notes[]={262, 294, 330};
void setup() {
  // put your setup code here, to run once:
  for (int i =0; i<3; i++){
    pinMode(pins[i], INPUT);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i =0; i<3; i++){
    if (digitalRead(pins[i]) == HIGH){
      tone(8, notes[i],100);
      delay(100);
      noTone(8);
    }
  }
}
