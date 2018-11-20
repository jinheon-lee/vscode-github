
#include <Stepper.h>

const int stepsPerRevolution = 200;
Stepper myStepper(stepsPerRevolution, 8,9,10,11);

void setup() 
{
  myStepper.setSpeed(100); 
}

void loop() 
{
  int i;
  
  for(i=0;i<200;i++)
  {
	myStepper.step(1);
	delay(10);
  }  
  
  for(i=0;i<200;i++)
  {
	myStepper.step(-1);
	delay(10);
  }  
}
