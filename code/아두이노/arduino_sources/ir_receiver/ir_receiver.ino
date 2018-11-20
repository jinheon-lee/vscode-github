

/*
value=20DF48B7
decode_type=1
value=20DFC837
decode_type=1
value=20DF28D7
decode_type=1
value=20DFA857
decode_type=1
value=20DF6897
decode_type=1
value=20DFE817
decode_type=1
value=20DF18E7
decode_type=1
value=20DF9867
decode_type=1
value=8789C683
decode_type=-1
value=8E1C1C14
decode_type=-1
*/

/*
  IR_remote_detector sketch
  An IR remote receiver is connected to pin 2.
  The LED on pin 13 toggles each time a button on the remote is pressed.
 */

#include <IRremote.h>                    //adds the library code to the sketch

const int irReceiverPin = 2;             //pin the receiver is connected to
const int ledPin = 12;

IRrecv irrecv(irReceiverPin);            //create an IRrecv object
decode_results decodedSignal;            //stores results from IR detectorvoid setup()

void setup()
{
  // Open serial communications and wait for port to open:
   Serial.begin(9600);
   while (!Serial) {
   ; // wait for serial port to connect. Needed for Leonardo only
   }
     
  pinMode(ledPin, OUTPUT);
  irrecv.enableIRIn();                   // Start the receiver object
}

boolean lightState = false;     //keep track of whether the LED is on
unsigned long last = millis();  //remember when we last received an IR message

/*
// decode type
#define NEC 1  --> Samsung remocorn
#define SONY 2
#define RC5 3
#define RC6 4
#define DISH 5
#define SHARP 6
#define PANASONIC 7
#define JVC 8
#define SANYO 9
#define MITSUBISHI 10
#define UNKNOWN -1
*/

void loop()
{


  if (irrecv.decode(&decodedSignal) == true) //this is true if a message has //been received                                             
  {
    if (millis() - last > 250/*250*/) 
    {       
      //has it been 1/4 sec since last message?
      lightState = !lightState;              //Yes: toggle the LED
      digitalWrite(ledPin, lightState);
      Serial.print("value=");
      Serial.println(decodedSignal.value, HEX);
      Serial.print("decode_type=");
      Serial.println(decodedSignal.decode_type);
    }
    
    last = millis();
    irrecv.resume();                         // watch out for another message
  }

}

