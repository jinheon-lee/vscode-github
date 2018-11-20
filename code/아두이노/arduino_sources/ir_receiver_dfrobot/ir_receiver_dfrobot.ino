#if 1

// dfrobot version

// Arduino IRDA Library
// https://github.com/shirriff/Arduino-IRremote


// 0.1 by pmalmsten http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1176098434
// 0.2 by farkinga
// 0.3 by farkinga - adds cool behaviors

/* 
0.4 by pstrobl    changed the original program to use on IR Kit For Arduino Model: DFR0107 32 bit controller.    
All keypresses are sent to the serial monitor at 9600 baud.   
pulseIn is always HIGH. The phototransistor in the kit does not invert the signal.   uses pin 13 for heartbeat debug   
32 bits requires a long variable, so divided up into two 15 bit so can use integer variables   
use the first 15 bits of the 32 bits for remote and data stream verification. This code is always the same for every button press   
use the last 15 of the 32 bits for button press selection. This code changes for each button.   ignore the middle 2 bits, it never changes.  
*/ 

// number of bits sent by IR remote
#define FirstLastBit 15     

// divide 32 bits into two 15 bit chunks for integer variables. Ignore center two bits. they are all the same.
#define IR_BIT_LENGTH 32
#define BIT_1 1000 /*1500*/          // Binary 1 threshold (Microseconds)
#define BIT_0 400 /*450*/           // Binary 0 threshold (Microseconds)
#define BIT_START 2500/*4000*/      // Start bit threshold (Microseconds) 

#define IR_PIN 2            // IR Sensor pin
#define LED_PIN 12          // LED goes off when signal is received 

int debug = 0;              
// flag as 1 to output raw IR pulse data stream length in microseconds

int output_verify = 0;      
// flag as 1 to print decoded verification integers. same number for all buttons

int output_key = 0;         
// flag as 1 to print decoded key integers

int remote_verify = 16128;  
// verifies first bits are 11111100000000 different remotes may have different start codes 

void setup() 
{  
 pinMode(LED_PIN, OUTPUT);
//This shows when ready to recieve  
pinMode(IR_PIN, INPUT);  
digitalWrite(LED_PIN, LOW);  
Serial.begin(9600);
} 

void loop() 
{  
digitalWrite(LED_PIN, HIGH);  
int key = get_ir_key();     
digitalWrite(LED_PIN, LOW);  

// turn LED off while processing response  
do_response(key);  
delay(130);                 
// 2 cycle delay to cancel duplicate keypresses
} 

/*  wait for a keypress from the IR remote, and return the  integer mapping of that key (e.g. power button on remote returns   the integer 1429)*/ 

int get_ir_key() 
{  
int pulse[IR_BIT_LENGTH];  
int bits[IR_BIT_LENGTH];   
do {} //Wait for a start bit  
while(pulseIn(IR_PIN, HIGH) < BIT_START);   

read_pulse(pulse);  
pulse_to_bits(pulse, bits);  
RemoteVerify(bits);  
return bits_to_int(bits);
}  
/*  use pulseIn to receive IR pulses from the remote.  Record the length of these pulses (in ms) in an array*/ 

void read_pulse(int pulse[])
{  
  for (int i = 0; i < IR_BIT_LENGTH; i++)  
  {    
    pulse[i] = pulseIn(IR_PIN, HIGH);  
  }
} /*  IR pulses encode binary "0" as a short pulse, and binary "1"  as a long pulse.  Given an array containing pulse lengths,  convert this to an array containing binary values*/ 

void pulse_to_bits(int pulse[], int bits[])
{  
  if (debug) 
  { 
    Serial.println("-----"); 
  }  

  for(int i = 0; i < IR_BIT_LENGTH; i++)   
  {    
    if (debug) 
    { 
      Serial.println(pulse[i]); 
    }    
    if(pulse[i] > BIT_1) //is it a 1?    
    {      
      bits[i] = 1;    
    }      
    else if(pulse[i] > BIT_0) //is it a 0?    
    {      
      bits[i] = 0;    
    }     
    else //data is invalid...    
    {      
      Serial.println("Error");    
    }  
  }
} 

/*  check returns proper first 14 check bits*/ 
void RemoteVerify(int bits[])
{  
  int result = 0;  
  int seed = 1;     //Convert bits to integer  

  for(int i = 0 ; i < (FirstLastBit) ; i++)  
  {           
    if(bits[i] == 1)     
    {    
      result += seed;    
    }         
    seed *= 2;  
  }        

  if (output_verify)      
  {        
    Serial.print("Remote ");        
    Serial.print(result);        
    Serial.println(" verification code");      
  } 

  if (remote_verify != result) 
  {
    delay (60); 
    get_ir_key();
  } 

  //verify first group of bits. delay for data stream to end, then try again.
}  

/*  convert an array of binary values to a single base-10 integer*/ 
int bits_to_int(int bits[])
{  
int result = 0;  
int seed = 1;     

  //Convert bits to integer  
  for(int i = (IR_BIT_LENGTH-FirstLastBit) ; i < IR_BIT_LENGTH ; i++)   
  {           
    if(bits[i] == 1)     
    {    
      result += seed;    
    }       
  
    seed *= 2;  
  }  
  return result;
}  

/*   respond to specific remote-control keys with different behaviors*/ 

void do_response(int key)
{       
  if (output_key)   
  {      
    Serial.print("Key ");      
    Serial.println(key);   
  }     

  switch (key)  
  {    
    case 32640:  // turns on UUT power      
      Serial.println("POWER");      
      break;     
    case 32385:  // FUNC/STOP turns off UUT power      
      Serial.println("FUNC/STOP");      
      break;
    default:      
    {        
      Serial.print("Key ");        
      Serial.print(key, HEX);        
      Serial.println(" not programmed");      
    }    
    break;  
  }
}


#endif
