
#define DEBUG 1

#include <IRremote.h>
 
IRsend irsend; 
int incomingByte = 0;
 
void setup()
{
  Serial.begin(9600);
}
 
void loop() 
{ 
   irsend.sendNEC( 0xc5000000, 8); 
    delay(1000);
    irsend.sendNEC( 0x45000000, 8); 
    delay(1000);
    irsend.sendNEC( 0x25000000, 8); 
    delay(1000);
    irsend.sendNEC( 0x85000000, 8); 
    delay(1000);
   
}
 
