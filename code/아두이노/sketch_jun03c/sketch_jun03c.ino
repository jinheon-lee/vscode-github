#include <Servo.h>
Servo myservo;

void setup() {
  // put your setup code here, to run once:
myservo.attach(9);
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i=0; i<120; ++i)
  {
    myservo.write(i);
    delay (15);
  }
  myservo.write(0);
  delay (1000); 
}
