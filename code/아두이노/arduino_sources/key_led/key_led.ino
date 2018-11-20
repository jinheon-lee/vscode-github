int led1 = 7;
int led2 = 6;

int key1 = 13;
int key2 = 12;

void setup()
{
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);

  pinMode(key1, INPUT);
  pinMode(key2, INPUT);   
  
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);  
}

void loop()
{
  if( digitalRead(key1) == HIGH )
    digitalWrite(led1, HIGH);
  else
    digitalWrite(led1, LOW);  
    
  if( digitalRead(key2) == HIGH )
    digitalWrite(led2, HIGH);
  else
    digitalWrite(led2, LOW);   
  
  delay(100);
}

