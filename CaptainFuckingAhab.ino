#include <PVision.h>

#include <Servo.h>
#include <Wire.h>


PVision greenCam;


int ledPin = 13;

void setup()
{
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  greenCam.init();
}

void loop()
{
  
}

void TestLEDBlink()
{
  digitalWrite(ledPin, HIGH);
  delay(1000);
  digitalWrite(ledPin, LOW);
  delay(1000);
}
