
/* XBee 수신용 프로그램 */

// Pin 13 has an LED connected on most Arduino boards.
int led = 13;

boolean ack = true;

int  serIn;             // var that will hold the bytes-in read from the serialBuffer
char serInString[100];  // array that will hold the different bytes  100=100characters;
                        // -> you must state how long the array will be else it won't work.
int  serInIndx  = 0;    // index of serInString[] in which to insert the next incoming byte
int  serOutIndx = 0;    // index of the outgoing serInString[] array;

void setup() {    
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT); 
  
  Serial.begin(9600);
}

void loop() 
{   
    readSerialString();
    
    if( serInIndx > 4 )
    {      
      if( strncmp(serInString, "REQ", 2)  == 0 )
      {
        // Read XBee "REQ\Cr\Lf" Data

        
        digitalWrite(led, HIGH);   // turn the LED on
        delay(1000);               // wait for a second   
        
        Serial.print("ACK");
        Serial.println();

        digitalWrite(led, LOW);   // turn the LED off
        delay(1000);               // wait for a second
      }
      serOutIndx = 0;
      serInIndx  = 0;
    }
}

void readSerialString () 
{
    int sb;   
    if(Serial.available()) { 
       while (Serial.available()){ 
          sb = Serial.read();             
          serInString[serInIndx] = sb;
          serInIndx++;
          //serialWrite(sb);                        //optional confirmation
       }
       //Serial.println();
    }  
}

void printSerialString() 
{
   if( serInIndx > 0) {
      //loop through all bytes in the array and print them out
      for(serOutIndx=0; serOutIndx < serInIndx; serOutIndx++) {
          Serial.print( serInString[serOutIndx] );    //print out the byte at the specified index
          //serInString[serOutIndx] = "";            //optional: flush out the content
      }        
      //reset all the functions to be able to fill the string back with content
      serOutIndx = 0;
      serInIndx  = 0;
      Serial.println();
   }
}

