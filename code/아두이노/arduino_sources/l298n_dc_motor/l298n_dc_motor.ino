
int in_a = 2;
int in_b = 4;
int en_a = 5;

void setup() 
{
  pinMode(in_a, OUTPUT);
  pinMode(in_b, OUTPUT);
}

void loop() 
{
  // Forward
  digitalWrite(in_a, HIGH);
  digitalWrite(in_b, LOW);
  analogWrite(en_a, 50); 
  
  delay(2000);
  
  // Backward
  digitalWrite(in_a, LOW);
  digitalWrite(in_b, HIGH);
  analogWrite(en_a, 250); 
  
  delay(2000);  
}
