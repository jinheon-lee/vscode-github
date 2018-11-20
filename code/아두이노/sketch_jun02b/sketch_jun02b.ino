#include <boarddefs.h>
#include <IRremote.h>
#include <IRremoteInt.h>
#include <ir_Lego_PF_BitStreamEncoder.h>

#include <Servo.h>

int IRpin=9;
int angle=0;

Servo myservo;
IRrecv irrecv(IRpin);
decode_results results;
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
irrecv.enableIRIn();
myservo.attach(8);
}

void loop() {
  // put your main code here, to run repeatedly:
if(irrecv.decode(&results))
{
  Serial.println(results.value, HEX);
  Serial.println(results.value);
  irrecv.resume();
}
if(results.value == 16724175)
{
  myservo.write(180);
}
if (results.value==16718055)
{
  myservo.write(0);  
}
}
