#include <ServoTimer2.h>

//int servoPin = 3;
int servoTestPin = 3;
//int servoTestPin2 = 11;

//ServoTimer2 grabberServo;
ServoTimer2 testServo;
//ServoTimer2 testServo2;

void setup() {
  // put your setup code here, to run once:
  //grabberServo.attach(servoPin);
  testServo.attach(servoTestPin);
  //testServo2.attach(servoTestPin2);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  
  
  StopInPosition();
  /*
  for(int x = 600; x < 2400; x++)
  {
    //grabberServo.write(x);
    testServo.write(x);
    //testServo2.write(x);
    //Serial.println(x);
    delay(1);
  }
  for(int x = 2400; x >= 600; x--)
  {
    //grabberServo.write(x);
    testServo.write(x);
    //testServo2.write(x);
    //Serial.println(x);
    delay(1);
  }
  */
}

void StopInPosition()
{
  testServo.write((2400+600)/2);
}
