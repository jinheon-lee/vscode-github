
int led1 = 7;
int led2 = 6;

int led1_status = LOW; // LED1 상태 
int led2_status = LOW; // LED2 상태

void setup()
{
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);

  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);  
  
  Serial.begin(9600);
}

void loop()
{
  char read_data;
  
  if (Serial.available())
  {
    read_data = Serial.read();
    
    if( read_data == '1' && led1_status == LOW)
    {
      digitalWrite(led1, HIGH);
      led1_status = HIGH;
      Serial.println("LED1 ON");  
    }
    else if( read_data == '1' && led1_status == HIGH )
    {
      digitalWrite(led1, LOW);  
      led1_status = LOW;
      Serial.println("LED1 OFF");  
    }
  
    if( read_data == '2' && led2_status == LOW)
    {
      digitalWrite(led2, HIGH);
      led2_status = HIGH;
      Serial.println("LED2 ON");  
    }
    else if( read_data == '2' && led2_status == HIGH )
    {
      digitalWrite(led2, LOW);  
      led2_status = LOW;
      Serial.println("LED1 OFF");
    }    
  }  
  delay(10);
}
