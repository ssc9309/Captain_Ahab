//don't forget to take out all serial.print when finished


//The rover will go straight
//Until wall is found.
//Then turn until no wall is seen
//Stop motor if rover is tilted, and turn servo
//Talk dirty to Pequod.

#include <Wire.h>
#include <VirtualWire.h>
#include <ServoTimer2.h>
//#include <NewPing.h>
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
int m0RPin = 11;
int m1RPin = 10;
int stepRPin = 9;
int dirRPin = 8;

int m0LPin = 4;
int m1LPin = 5;
int stepLPin = 6;
int dirLPin = 7;

//Ultrasonic Variables
int trigPin = A5;
int echoPin = A4;
int distanceInCm = -1;

//tilt sensor Variables
int tiltPin = A3;

//Servo variables
int servoPin = 3;
ServoTimer2 grabberServo;

//Transmitter Variables;
int transPin = 2;


void setup() 
{
  // put your setup code here, to run once:
  
  Serial.begin(9600);
//  Serial.println("Setup Start");
  
  
  ThreadingSetup();
  StepperSetup();
  SonarSetup();
  TiltSetup();
  ServoSetup();
  TransmitterSetup();
  
  
//  Serial.println("Setup Done");
}

void ThreadingSetup()
{
//  Serial.println("Threading Setup Start");
  
  PT_INIT(&pt1);
  PT_INIT(&pt2);
  PT_INIT(&pt3);
  
//  Serial.println("Threading Setup Done");
}

void StepperSetup()
{
//  Serial.println("Stepper Setup Start");
  
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
  
//  Serial.println("Stepper Setup Done");
}

void SonarSetup()
{
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void TiltSetup()
{
  pinMode(tiltPin, INPUT);
}

void ServoSetup()
{
  //pinMode(servoPin, OUTPUT);
  grabberServo.attach(servoPin);
  
  //ex. grabberServo.write(pulsewidth in microseconds); 600 - 2400
}

void TransmitterSetup()
{
  vw_set_ptt_inverted(true); // Required for DR3100
  vw_set_tx_pin(transPin);
  vw_setup(2000);	 // Bits per sec
}


//------------------Functional code----------------------------
void loop() 
{
  // put your main code here, to run repeatedly:
  AhabMovementThread(&pt1);
  CheckSonarThread(&pt2);

/*  
  //if it's tilted, stop
  if (digitalRead(tiltPin) == HIGH)
  {
    if (stepperDirection != 's')
    {
      stepperDirection = 's';
      SendDataToPequod("hss");
    }
    TurnServo();
  }
*/
  //if there was a distance feedback
  //else if (distanceInCm != -1)
  if (distanceInCm != -1)
  {
    //if the distance was less than 30 cm, turn left.
    if (distanceInCm < 30)
    {
      if (stepperDirection != 'l')
      {
        stepperDirection = 'l';
        SendDataToPequod("hsl");
      }    
    }
    else
    {
      if (stepperDirection != 'f')
      {
        stepperDirection = 'f';
        SendDataToPequod("hsf");
      }
    }
  }
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
//      Serial.print(millis());
//      Serial.println(": Direction Forward");
      digitalWrite(dirRPin, LOW);
      digitalWrite(dirLPin, HIGH);
    }
    else if (stepperDirection == 'r')
    {
//      Serial.print(millis());
//      Serial.println(": Direction Right");
      digitalWrite(dirRPin, HIGH);
      digitalWrite(dirLPin, HIGH);
    }
    else if (stepperDirection == 'l')
    {
//      Serial.print(millis());
//      Serial.println(": Direction Left");
      digitalWrite(dirRPin, LOW);
      digitalWrite(dirLPin, LOW);
    }
    else if (stepperDirection == 'b')
    {
//      Serial.print(millis());
//      Serial.println(": Direction Back");
      digitalWrite(dirRPin, LOW);
      digitalWrite(dirLPin, LOW);
    }
    else
    {
//      Serial.print(millis());
//      Serial.println(": I am lost in setting direction");
    }
    
    return true;
  }
  else
  {
    return false;
  }
}

static int CheckSonarThread(struct pt *pt)
{
  static unsigned long pingTime = 0;
  static unsigned long echoTime = 0;
  distanceInCm = -1;
  static bool pinged = false;
  
  PT_BEGIN(pt);
  
  //send a ping only if a ping hasn't been sent yet  
  if (!pinged)
  {
    pinged = true;
    digitalWrite(trigPin, HIGH);
    pingTime = micros();
  }
  else
  {
    //100000 us is longer than 7 meters
    if (micros() - pingTime > 100000)
    {
      Serial.println("Out of range (7m)");
      pinged = false;
    }
    
    PT_WAIT_UNTIL(pt, digitalRead(echoPin) == HIGH);
    echoTime = micros();
    pinged = false;
    
    distanceInCm = (echoTime - pingTime)/2;
    distanceInCm /= 1000000;
    distanceInCm *= 34000;
    
    Serial.print("Distance: ");
    Serial.print(distanceInCm);
    Serial.println(" cm");
  }
  //PT_WAIT_UNTIL(pt, micros() - timeStamp > stepperSpeed);
  
  PT_END(pt);
}

void TurnServo()
{
  for(int x = 600; x < 2400; x++)
  {
    grabberServo.write(x);
    delay(1);
  }
  for(int x = 2400; x >= 600; x--)
  {
    grabberServo.write(x);
    delay(1);
  }
}

//string
//always have two letters as hs
void SendDataToPequod(char *msg)
{
  vw_send((uint8_t *)msg, strlen(msg));
  vw_wait_tx(); // Wait until the whole message is gone
}
