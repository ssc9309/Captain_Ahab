#include <ServoTimer2.h>

int servoPin = 7;
//0 = off
//1 = full +
//2 = off
//3 = full -
int servoState = 0; 
int buttonPin = A4;
int prevButtonState = 0;


ServoTimer2 servo;

void setup() {
  // put your setup code here, to run once:
  servo.attach(servoPin);
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
}

void loop() 
{
  int currentButtonState = digitalRead(buttonPin);
  if (currentButtonState != prevButtonState)
  {
    prevButtonState = currentButtonState;
    
    if (currentButtonState == 1)
    {
      servoState++;
      Serial.println(servoState);
      servoState %= 4;
    }
  }
  
  if (servoState == 0 || servoState == 2)
  {
    servo.write((2400+600)/2);
  }
  else if (servoState == 1)
  {
    servo.write(2400);
  }
  else
  {
    servo.write(600);
  }
  
  // put your main code here, to run repeatedly:

}
