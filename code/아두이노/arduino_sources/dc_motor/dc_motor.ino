
#define motor1EnablePin 9
#define motor1_1        2
#define motor1_2        4

void setup()
{
  pinMode(motor1_1, OUTPUT);
  pinMode(motor1_2, OUTPUT);
  analogWrite(motor1EnablePin, 0);
}

void loop()
{
  digitalWrite(motor1_1, HIGH);
  digitalWrite(motor1_2, LOW);  
  analogWrite(motor1EnablePin, 1023);
  delay(2000);
  analogWrite(motor1EnablePin, 50);
  analogWrite(motor1EnablePin, 0);
  delay(1000);
  
  digitalWrite(motor1_1, LOW);
  digitalWrite(motor1_2, HIGH);  
  analogWrite(motor1EnablePin, 128);
  delay(2000);
  analogWrite(motor1EnablePin, 50);
  analogWrite(motor1EnablePin, 0);
  delay(1000);  
}
