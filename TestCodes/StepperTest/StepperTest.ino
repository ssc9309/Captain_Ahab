int m0RPin = 4;
int m1RPin = 5;
int stepRPin = 2;
int dirRPin = 3;

int m0LPin = 9;
int m1LPin = 8;
int stepLPin = 7;
int dirLPin = 6;


void setup() {
  // put your setup code here, to run once:

  pinMode(m0RPin, HIGH);
  pinMode(m1RPin, HIGH);
  pinMode(stepRPin, HIGH);
  pinMode(dirRPin, HIGH);
  
  digitalWrite(m0RPin, LOW);
  digitalWrite(m1RPin, LOW);
  digitalWrite(dirRPin, HIGH);
  
  pinMode(m0LPin, HIGH);
  pinMode(m1LPin, HIGH);
  pinMode(stepLPin, HIGH);
  pinMode(dirLPin, HIGH);
  
  digitalWrite(m0LPin, LOW);
  digitalWrite(m1LPin, LOW);
  digitalWrite(dirLPin, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(stepRPin, HIGH);
  digitalWrite(stepLPin, HIGH);
  delay(10);
  digitalWrite(stepRPin, LOW);
  digitalWrite(stepLPin, LOW);
  delay(10);
}
