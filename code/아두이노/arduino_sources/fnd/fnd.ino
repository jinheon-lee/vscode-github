
const int _a=2;
const int _b=3;
const int _c=4;
const int _d=5;
const int _e=9;
const int _f=8;
const int _g=7;
const int _dot=6;

void display_number(int n)
{
 
  switch(n)
  {
    case 0:
      digitalWrite(_e,HIGH);
      digitalWrite(_b,HIGH);
      digitalWrite(_c,HIGH);
      digitalWrite(_d,HIGH);
      digitalWrite(_g,HIGH);
      digitalWrite(_f,HIGH);
      digitalWrite(_a,LOW);
      digitalWrite(_dot,LOW);
      break;       
    case 1:
      digitalWrite(_e,LOW);
      digitalWrite(_b,LOW);
      digitalWrite(_c,LOW);
      digitalWrite(_d,LOW);
      digitalWrite(_g,HIGH);
      digitalWrite(_f,HIGH);
      digitalWrite(_a,LOW);
      digitalWrite(_dot,LOW);
      break;    
    case 2:
      digitalWrite(_e,HIGH);
      digitalWrite(_b,LOW);
      digitalWrite(_c,HIGH);
      digitalWrite(_d,HIGH);
      digitalWrite(_g,LOW);
      digitalWrite(_f,HIGH);
      digitalWrite(_a,HIGH);
      digitalWrite(_dot,LOW);
      break;  
    case 3:
      digitalWrite(_e,HIGH);
      digitalWrite(_b,LOW);
      digitalWrite(_c,LOW);
      digitalWrite(_d,HIGH);
      digitalWrite(_g,HIGH);
      digitalWrite(_f,HIGH);
      digitalWrite(_a,HIGH);
      digitalWrite(_dot,LOW);
      break;
    case 4:
      digitalWrite(_e,LOW);
      digitalWrite(_b,HIGH);
      digitalWrite(_c,LOW);
      digitalWrite(_d,LOW);
      digitalWrite(_g,HIGH);
      digitalWrite(_f,HIGH);
      digitalWrite(_a,HIGH);
      digitalWrite(_dot,LOW);
      break;    
    case 5:
      digitalWrite(_e,HIGH);
      digitalWrite(_b,HIGH);
      digitalWrite(_c,LOW);
      digitalWrite(_d,HIGH);
      digitalWrite(_g,HIGH);
      digitalWrite(_f,LOW);
      digitalWrite(_a,HIGH);
      digitalWrite(_dot,LOW);
      break;
    case 6:
      digitalWrite(_e,HIGH);
      digitalWrite(_b,HIGH);
      digitalWrite(_c,HIGH);
      digitalWrite(_d,HIGH);
      digitalWrite(_g,HIGH);
      digitalWrite(_f,LOW);
      digitalWrite(_a,HIGH);
      digitalWrite(_dot,LOW);
      break;    
    case 7:
      digitalWrite(_e,HIGH);
      digitalWrite(_b,HIGH);
      digitalWrite(_c,LOW);
      digitalWrite(_d,LOW);
      digitalWrite(_g,HIGH);
      digitalWrite(_f,HIGH);
      digitalWrite(_a,LOW);
      digitalWrite(_dot,LOW);
      break;    
    case 8:
      digitalWrite(_e,HIGH);
      digitalWrite(_b,HIGH);
      digitalWrite(_c,HIGH);
      digitalWrite(_d,HIGH);
      digitalWrite(_g,HIGH);
      digitalWrite(_f,HIGH);
      digitalWrite(_a,HIGH);
      digitalWrite(_dot,LOW);
      break;    
    case 9:
      digitalWrite(_e,HIGH);
      digitalWrite(_b,HIGH);
      digitalWrite(_c,LOW);
      digitalWrite(_d,HIGH);
      digitalWrite(_g,HIGH);
      digitalWrite(_f,HIGH);
      digitalWrite(_a,HIGH);
      digitalWrite(_dot,LOW);
      break; 
    case 10:
      digitalWrite(_e,LOW);
      digitalWrite(_b,LOW);
      digitalWrite(_c,LOW);
      digitalWrite(_d,LOW);
      digitalWrite(_g,LOW);
      digitalWrite(_f,LOW);
      digitalWrite(_a,LOW);
      digitalWrite(_dot,HIGH);
      break;       
  }
}

void setup()
{
  pinMode(_a,OUTPUT);
  pinMode(_b,OUTPUT);
  pinMode(_c,OUTPUT);
  pinMode(_d,OUTPUT);
  pinMode(_e,OUTPUT);
  pinMode(_f,OUTPUT);
  pinMode(_g,OUTPUT);
  pinMode(_dot,OUTPUT);  
};

void loop()
{
  int i;
  
  for(i=0;i<11;i++)
  {
    display_number(i);
    delay(1000);
  }
}
