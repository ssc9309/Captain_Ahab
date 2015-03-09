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

  pinMode(m0RPin, OUTPUT);
  pinMode(m1RPin, OUTPUT);
  pinMode(stepRPin, OUTPUT);
  pinMode(dirRPin, OUTPUT);
  
  digitalWrite(m0RPin, LOW);
  digitalWrite(m1RPin, LOW);
  digitalWrite(dirRPin, HIGH);
  
  pinMode(m0LPin, OUTPUT);
  pinMode(m1LPin, OUTPUT);
  pinMode(stepLPin, OUTPUT);
  pinMode(dirLPin, OUTPUT);
  
  digitalWrite(m0LPin, LOW);
  digitalWrite(m1LPin, LOW);
  digitalWrite(dirLPin, HIGH);
}

void loop() 
{
  // put your main code here, to run repeatedly:
  digitalWrite(stepRPin, HIGH);
  digitalWrite(stepLPin, HIGH);
  delay(1);
  //delayMicroseconds(125);
  
  digitalWrite(stepRPin, LOW);
  digitalWrite(stepLPin, LOW);
  delay(1);
  //delayMicroseconds(125);
}
