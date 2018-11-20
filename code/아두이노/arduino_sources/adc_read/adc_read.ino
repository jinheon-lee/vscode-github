 
int sensorPin = A0;    // select the input pin for the potentiometer
int led = 9;           // the pin that the LED is attached to
 
// the setup routine runs once when you press reset:
 void setup()  
 { 
  // declare pin 9 to be an output:
   pinMode(led, OUTPUT);
   
   Serial.begin(9600);
 } 

// the loop routine runs over and over again forever:
void loop()  
{ 
  int sensorValue = 0;
  
  // read the value from variable resistence
  sensorValue = analogRead(sensorPin);    
  
  analogWrite(led, sensorValue);
  Serial.println(sensorValue);
  
  // wait for 10 milliseconds to see the dimming effect    
  delay(10);                            
}
