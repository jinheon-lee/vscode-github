
int DI = 12;
int RW = 11;
int DB[] = {3, 4, 5, 6, 7, 8, 9, 10};
int Enable = 2;

void LcdCommandWrite(int value) {
 // poll all the pins
 int i = 0;
 for (i=DB[0]; i <= DI; i++) {
   digitalWrite(i,value & 01);
   value >>= 1;
 }
 digitalWrite(Enable,LOW);
 delayMicroseconds(1);
 // send a pulse to enable
 digitalWrite(Enable,HIGH);
 delayMicroseconds(1);  // pause 1 ms according to datasheet
 digitalWrite(Enable,LOW);
 delayMicroseconds(1);  // pause 1 ms according to datasheet
}

void LcdDataWrite(int value) {
 // poll all the pins
 int i = 0;
 digitalWrite(DI, HIGH);
 digitalWrite(RW, LOW);
 for (i=DB[0]; i <= DB[7]; i++) {
   digitalWrite(i,value & 01);
   value >>= 1;
 }
 digitalWrite(Enable,LOW);
 delayMicroseconds(1);
 // send a pulse to enable
 digitalWrite(Enable,HIGH);
 delayMicroseconds(1);
 digitalWrite(Enable,LOW);
 delayMicroseconds(1);  // pause 1 ms according to datasheet
}

void initLCD() {
 delay(100);
 // initiatize lcd after a short pause
 // needed by the LCDs controller
 LcdCommandWrite(0x3A);  // function set:
                         // 8-bit interface, 2 display lines, display ON
 delay(10);                     
 LcdCommandWrite(0x0E);  // display control:
                         // turn display on, cursor on, no blinking
 delay(10);                     
 LcdCommandWrite(0x01);  // clear display, set cursor position to zero 
 delay(10);                     
 LcdCommandWrite(0x06);  // entry mode set:
                         // increment automatically, no display shift
 delay(10);                     
}

// this is the function used to send data to the
// LCD screen in the proper format, the others are
// working at lower level
void printLCD(const char *s) {
  int count = 0;
  while (*s) {
    if(count==8) {
      LcdCommandWrite(0xC0);  // jump to the second part of the display:
      delay(5);               
    }
    if(count>=16) {
      break;
      }
    LcdDataWrite(*s++);
    count ++;
  }
}

void setup () {
 int i = 0;
 for (i=Enable; i <= DI; i++) {
   pinMode(i,OUTPUT);
 }
 initLCD();
}

void loop () {
  LcdCommandWrite(0x02);  // set cursor position to zero 
  delay(10);                    
  // Write the welcome message
  printLCD("Hellow World!");
  delay(500);
}

