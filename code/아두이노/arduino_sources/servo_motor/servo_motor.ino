
#include <Servo.h>

int motor_control = 8;
Servo servo;

void setup()
{
  servo.attach(motor_control);
}

void loop()
{
  int i;
  
  servo.write(0);
  delay(1000);
  
  for(i=0;i<90;i+=10)
  {
    servo.write(i);
    delay(20);
  }
  
  delay(1000);
}
