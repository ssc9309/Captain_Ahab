#include <ServoTimer2.h>

int servoPin = 10;

ServoTimer2 grabberServo;

void setup() {
  // put your setup code here, to run once:
  grabberServo.attach(servoPin);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int x = 500; x < 2500; x++)
  {
    grabberServo.write(x);
    //Serial.println(x);
    delay(1);
  }
  for(int x = 2500; x >= 500; x--)
  {
    grabberServo.write(x);
    //Serial.println(x);
    delay(1);
  }
}
