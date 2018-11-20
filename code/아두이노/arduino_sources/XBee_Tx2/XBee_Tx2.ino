/* 특정 XBee 에만 송신하는 프로그램 */

// Pin 13 has an LED connected on most Arduino boards.
int led = 13;
boolean is_xbee_configured = false;

boolean ack = true;

int  serIn;             // var that will hold the bytes-in read from the serialBuffer
char serInString[100];  // array that will hold the different bytes  100=100characters;
                        // -> you must state how long the array will be else it won't work.
int  serInIndx  = 0;    // index of serInString[] in which to insert the next incoming byte
int  serOutIndx = 0;    // index of the outgoing serInString[] array;

boolean response_xbee()
{
  String response_ok = "OK\r";
  String response = String("");
  
  while( response.length() < response_ok.length() )
  {    
    if( Serial.available() > 0 )
    {
      response += (char)Serial.read();
    }
  }
  
  if( response.equals(response_ok))
  {
    // digitalWrite(led, HIGH);
    return true;
  }
  else
  {
    // digitalWrite(led, LOW);
    return false;
  }
}

boolean configure_xbee()
{
  // XBee를  명령 모드로 진입 시킨다.
  Serial.print("+++");
  
  if( response_xbee() )
  {
    Serial.print("ATDH13A200\r");
    Serial.print("ATDL409BE128\r");
    Serial.print("ATCN\r");  
  }
  else
  {
    return false;  
  }
    
  return true;
}

// the setup routine runs once when you press reset:
void setup() 
{                
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);     
  Serial.begin(9600);
  digitalWrite(led, LOW);
  
  is_xbee_configured = configure_xbee();

}

void loop()
{

  if( is_xbee_configured )
  {    
    if( ack == true )
    {
      digitalWrite(led, HIGH);   // turn the LED on
      delay(1000);               // wait for a second  
      
      Serial.print("REQ");
      Serial.println();
      
      ack = false;
    }
   
    readSerialString();
    
    if( serInIndx > 4 )
    {      
      if( strncmp(serInString, "ACK", 2)  == 0 )
      {
        // Read XBee "ACK\Cr\Lf" Data 
        ack = true;
        
        digitalWrite(led, LOW);   // turn the LED off
        delay(1000);               // wait for a second          
      }
      serOutIndx = 0;
      serInIndx  = 0;      
    }
  }
  else
  {    
    delay(2000);
    
    is_xbee_configured = configure_xbee();  // Retry configure
  }

}  

void readSerialString () 
{
    int sb;   
    if(Serial.available()) { 
       // Serial.print("reading Serial String: ");     //optional confirmation
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
      
   }
}

