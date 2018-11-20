#include <SoftwareSerial.h>
SoftwareSerial BTSerial(2, 3);
void setup() {
pinMode(9,OUTPUT);
BTSerial.begin(9600);
}

void loop() {
if(BTSerial.available()){ // 만약 BTSerial에 데이터의 입력이 있으면 
 
 int  a = BTSerial.read(); // int형 a에 입력값을 저장한다
 
  if(a == '1') // 만약 입력값이 1이면
  {
    digitalWrite(9,HIGH); //LED를 켠다
  }
  if(a == '2') // 만약 입력값이 2이면 
  {
    digitalWrite(9,LOW);// LED를 끈다
  }
 
  }
}
