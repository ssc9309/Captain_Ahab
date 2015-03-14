//The rover will go straight
//Until wall is found.
//Then turn until no wall is seen
//Also continuously update blob data
//Stop motor if rover is tilted, and turn servo
//Talk dirty to Pequod.

#include <Wire.h>
#include <PVision.h>
#include <VirtualWire.h>
#include <ServoTimer2.h>
#include <NewPing.h>
#include <pt.h>

//Threading variables

//pt1 = motordrive
static struct pt pt1, pt2, pt3;
int stepperSpeed = 350; //in us up to 1500 Hz (300 us)
char stepperDirection = 'f'; //s = stop, f = forward, r = right, l = left, b = back

//Stepper Variables

//m0 low and m1 low mean Full step.
//m0 high and m1 low mean Half step.
//m0 high and m1 high mean 1/16th step.
//Right LOW, left HIGH = straight
int m0RPin = 4;
int m1RPin = 5;
int stepRPin = 2;
int dirRPin = 3;

int m0LPin = 9;
int m1LPin = 8;
int stepLPin = 7;
int dirLPin = 6;

//Ultrasonic Variables
int trigPin = 12;
int echoPin = 11;


void setup() 
{
  // put your setup code here, to run once:
  
  Serial.begin(9600);
  Serial.println("Setup Start");
  
  
  ThreadingSetup();
  StepperSetup();
  SonarSetup();
  
  Serial.println("Setup Done");
}

void ThreadingSetup()
{
  Serial.println("Threading Setup Start");
  
  PT_INIT(&pt1);
  PT_INIT(&pt2);
  PT_INIT(&pt3);
  
  Serial.println("Threading Setup Done");
}

void StepperSetup()
{
  Serial.println("Stepper Setup Start");
  
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
  
  Serial.println("Stepper Setup Done");
}

void SonarSetup()
{
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}


//------------------Functional code----------------------------
void loop() 
{
  // put your main code here, to run repeatedly:
  AhabMovementThread(&pt1);
  CheckSonar(&pt2);
  
}

static int AhabMovementThread(struct pt *pt)
{
  static bool pulseHigh = 0;
  static unsigned long timeStamp = 0;
  
  PT_BEGIN(pt);
  
  //every half pulse, change direction and provide PWM
  PT_WAIT_UNTIL(pt, micros() - timeStamp > stepperSpeed);
  
  //true if not stop
  if (SetStepperDirection())
  {
    if (!pulseHigh)
    {
      digitalWrite(stepRPin, HIGH);
      digitalWrite(stepLPin, HIGH);
      pulseHigh = true;
    }
    else
    {
      digitalWrite(stepRPin, LOW);
      digitalWrite(stepLPin, LOW);
      pulseHigh = false;
    }
  }
  
  timeStamp = micros();
  
  PT_END(pt);
}

//returns false when supposed to stop
bool SetStepperDirection()
{
  if (stepperDirection != 's')
  {
    if (stepperDirection == 'f')
    {
      Serial.print(millis());
      Serial.println(": Direction Forward");
      digitalWrite(dirRPin, LOW);
      digitalWrite(dirLPin, HIGH);
    }
    else if (stepperDirection == 'r')
    {
      Serial.print(millis());
      Serial.println(": Direction Right");
      digitalWrite(dirRPin, HIGH);
      digitalWrite(dirLPin, HIGH);
    }
    else if (stepperDirection == 'l')
    {
      Serial.print(millis());
      Serial.println(": Direction Left");
      digitalWrite(dirRPin, LOW);
      digitalWrite(dirLPin, LOW);
    }
    else if (stepperDirection == 'b')
    {
      Serial.print(millis());
      Serial.println(": Direction Back");
      digitalWrite(dirRPin, LOW);
      digitalWrite(dirLPin, LOW);
    }
    else
    {
      Serial.print(millis());
      Serial.println(": I am lost in setting direction");
    }
    
    return true;
  }
  else
  {
    return false;
  }
}

static int CheckSonar(struct pt *pt)
{
  static unsigned long pingTime = 0;
  static unsigned long echoTime = 0;
  int distanceInCm = -1;
  static bool pinged = false;
  
  PT_BEGIN(pt);
  
  PT_WAIT_UNTIL(pt, micros() - pingTime > stepperSpeed);
  
  //send a ping only if a ping hasn't been sent yet
  if (!pinged)
  {
    pinged = true;
    digitalWrite(trigPin, HIGH);
    pingTime = micros();
  }
  //PT_WAIT_UNTIL(pt, micros() - timeStamp > stepperSpeed);
  
  
  
  
  
  PT_END(pt);
}





