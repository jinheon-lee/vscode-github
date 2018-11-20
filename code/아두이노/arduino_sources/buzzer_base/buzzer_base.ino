
int buzzer = 8;

void setup()
{
  pinMode(buzzer, OUTPUT); 
}

void loop()
{

  digitalWrite(buzzer, HIGH);  // Buzzer on
  delay(1000);
  digitalWrite(buzzer, LOW); // Buzzer off
  delay(1000);

}
