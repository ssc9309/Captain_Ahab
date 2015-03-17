//m0 low and m1 low mean Full step.
//m0 high and m1 low mean Half step.
//m0 high and m1 high mean 1/16th step.
//m0 low and ml high mean 1/8th step




int m0RPin = 11;
int m1RPin = 10;
int stepRPin = 9;
int dirRPin = 8;

int m0LPin = 4;
int m1LPin = 5;
int stepLPin = 6;
int dirLPin = 7;


void setup() 
{
  // put your setup code here, to run once:

  Serial.begin(9600);
  
  pinMode(m0RPin, OUTPUT);
  pinMode(m1RPin, OUTPUT);
  pinMode(stepRPin, OUTPUT);
  pinMode(dirRPin, OUTPUT);
  
  digitalWrite(m0RPin, LOW);
  digitalWrite(m1RPin, LOW);
  digitalWrite(dirRPin, LOW);
  
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
  
  while (millis() < 10000)
  {
    Serial.println(analogRead(A1));
  
  // put your main code here, to run repeatedly:
    digitalWrite(stepRPin, HIGH);
    digitalWrite(stepLPin, HIGH);
    //delay(1);
    delayMicroseconds(300/4);
    //delayMicroseconds(300/8 + 2*analogRead(A1));
    //300/8 + 2*242 full step was the fastest speed
    //0.0007 / 2 = 0.00035
    digitalWrite(stepRPin, LOW);
    digitalWrite(stepLPin, LOW);
    //delay(1);
    delayMicroseconds(300/4);
    //delayMicroseconds(300/8 + 2*analogRead(A1));
  }
  
}
