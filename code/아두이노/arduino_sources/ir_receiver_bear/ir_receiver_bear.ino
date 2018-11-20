







// bear version
/**
 * Receives signal from IR remote control on pin 2 and displays the function to the serial monitor.
 * Writes state of control to EEPROM for recall when power is turned off.
 * Reads state of control from EEPROM when power is turned on.
 */

// #include <EEPROM.h>

//CONSTANTS
#define btnPower   149
#define btnMute    148
#define btnDown    147
#define btnUp      146
#define rate       4

#define start_bit  2000                //Start bit threshold (Microseconds)
#define bin_1      1000                //Binary 1 threshold (Microseconds)
#define bin_0      400                //Binary 0 threshold (Microseconds)
#define ir_pin     2                //Sensor pin 1 wired through a 220 ohm resistor
#define led_pin    12                //"Ready to Recieve" flag, not needed but nice
#define debug      1                 //Serial connection must be started to debug

//VARIABLES
int power  = 0;//EEPROM.read(0);  
int mute   = 1;//EEPROM.read(1);  
int volume = 2;//EEPROM.read(2);
int i      = 0;                    //general purpose counter 



void setup() {
  pinMode(led_pin, OUTPUT);          //This shows when we're ready to recieve
  pinMode(ir_pin, INPUT);
  digitalWrite(led_pin, LOW);          //not ready yet
  Serial.begin(9600);
  if(volume == 255) volume = 0;    //Don't start up at full volume
}

void loop() 
{
  //Fetch the key  
  int key = getIRKey();
  
  //Switch to print insructions to Serial Monitor
   switch (key) {     
    case btnPower:
      if(power == 0){
        power = 1;
        Serial.println("Turn On");
      } else {
        power = 0;
        Serial.println("Turn Off");
      }
      //EEPROM.write(0, power);
      delay (500); //Debounce switch
      break;
      
    case btnMute:    
      if(mute == 0){
        mute = 1;
        Serial.println("Mute On");
      } else {
        mute = 0;
        Serial.println("Mute Off");
      }
      //EEPROM.write(1, mute);
      delay (500); //Debounce switch
      break;
      
    case btnDown:
      if(volume > 0 && volume - rate > 0) {
        volume -= rate;
      } else {
        volume = 0;
      }
      Serial.print("Volume: ");
      Serial.println(volume);
      //EEPROM.write(2, volume);
      break;   
      
    case btnUp:
      if(volume < 255 && volume + rate <= 255) {
        volume += rate;
      } else {
        volume = 255;
      }
      Serial.print("Volume: ");
      Serial.println(volume);
      //EEPROM.write(2, volume);
      break; 
  }

}


int getIRKey() 
{
  int data[12];
  digitalWrite(led_pin, HIGH);         //Ok, i'm ready to recieve  
  
  while(pulseIn(ir_pin, LOW) < 2200) 
  { 
    //Wait for a start bit
  }
  
  for(i=0;i<11;i++)
  {
    data[i] = pulseIn(ir_pin, LOW);   //Start measuring bits, I only want low pulses
  }
  
  digitalWrite(led_pin, LOW);

  if(debug == 1) 
  {
    Serial.println("-----");
  }
  
  for(int i=0;i<11;i++) 
  {          
    //Parse them    
    if (debug == 1) 
    {
      Serial.println(data[i]);
    }
    
    if(data[i] > bin_1) 
    {          //is it a 1?
      data[i] = 1;
    } 
    else 
    {
      if(data[i] > bin_0) {        //is it a 0?
        data[i] = 0;
      } else {
        data[i] = 2;              //Flag the data as invalid; I don't know what it is!
      }
    }
    
  }

  for(i=0;i<11;i++) {              //Pre-check data for errors
    if(data[i] > 1) {
      return -1;                //Return -1 on invalid data
    }
  }

  int result = 0;
  int seed = 1;
  for(int i=0;i<11;i++) 
  {      //Convert bits to integer
    if(data[i] == 1) {
      result += seed;
    }
    seed = seed * 2;
  }
  return result;             //Return key number
}






