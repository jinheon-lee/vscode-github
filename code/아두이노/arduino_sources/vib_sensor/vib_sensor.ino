int led = 3;
int sensor = 2;

void setup()
{
  pinMode(led, OUTPUT);
  pinMode(sensor, INPUT);
}

void loop()
{
  if( digitalRead(sensor) == LOW )
    digitalWrite(led, HIGH);
  else
    digitalWrite(led, LOW);
}

