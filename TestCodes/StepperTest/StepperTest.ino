//m0 low and m1 low mean Full step.
//m0 high and m1 low mean Half step.
//m0 high and m1 high mean 1/16th step.



int m0RPin = 4;
int m1RPin = 5;
int stepRPin = 2;
int dirRPin = 3;

int m0LPin = 9;
int m1LPin = 8;
int stepLPin = 7;
int dirLPin = 6;


void setup() 
{
  // put your setup code here, to run once:

  pinMode(m0RPin, HIGH);
  pinMode(m1RPin, HIGH);
  pinMode(stepRPin, HIGH);
  pinMode(dirRPin, HIGH);
  
  digitalWrite(m0RPin, HIGH);
  digitalWrite(m1RPin, HIGH);
  digitalWrite(dirRPin, HIGH);
  
  pinMode(m0LPin, HIGH);
  pinMode(m1LPin, HIGH);
  pinMode(stepLPin, HIGH);
  pinMode(dirLPin, HIGH);
  
  digitalWrite(m0LPin, HIGH);
  digitalWrite(m1LPin, HIGH);
  digitalWrite(dirLPin, HIGH);
}

void loop() 
{
  // put your main code here, to run repeatedly:
  digitalWrite(stepRPin, HIGH);
  digitalWrite(stepLPin, HIGH);
  //delay(2);
  delayMicroseconds(125);
  digitalWrite(stepRPin, LOW);
  digitalWrite(stepLPin, LOW);
  
  //delay(2);
  delayMicroseconds(125);
}
