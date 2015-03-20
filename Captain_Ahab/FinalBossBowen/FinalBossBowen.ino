#include <NewPing.h>
#include <VirtualWire.h>


//system variable

double distanceR = -1;
double distanceL = -1;
double distanceF = -1;



//Stepper variables
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

char stepperDirection = 'r';

//ultrasonic variables.
int trig1Pin = A5;
int echo1Pin = A4;
int trig2Pin = A2;
int echo2Pin = A1;
int trig3Pin = 13;
int echo3Pin = 12;

int maxDistance = 700; //maximum distance you want to see in cm
long duration, distance;

NewPing sonarL(trig1Pin, echo1Pin, maxDistance);
NewPing sonarR(trig2Pin, echo2Pin, maxDistance);
NewPing sonarF(trig3Pin, echo3Pin, maxDistance);


//Transmitter Setep
int transmitterPin = 2;


//Tilt variable
int tiltPin = A3;



void setup() 
{
  // put your setup code here, to run once:
  
  Serial.begin(9600);
  Serial.println("Setup begin");
  
  
  StepperSetup();
 // UltrasonicSetup();
  TransmitterSetup();
  
  pinMode(tiltPin, INPUT);
  
  
  Serial.println("Setup done");
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

void UltrasonicSetup()
{
  pinMode(trig1Pin, OUTPUT);
  pinMode(echo1Pin, INPUT);
  pinMode(trig2Pin, OUTPUT);
  pinMode(echo2Pin, INPUT);
  pinMode(trig3Pin, OUTPUT);
  pinMode(echo3Pin, INPUT);
}

void TransmitterSetup()
{
  vw_set_ptt_inverted(true); // Required for DR3100
  vw_setup(2000);	 // Bits per sec
  vw_set_tx_pin(transmitterPin); //Pin 2 is connected to "Digital Output" of receiver
}



///////////////////////////////////Main code////////////////////////////////////
int initialTiltStateCount = 0;
bool searchingFirst = false;
//bool launchingFirst = false;
void loop() 
{
  // put your main code here, to run repeatedly:
  
  while (millis() < 5000)
  {
    if (digitalRead(tiltPin))
    {
      initialTiltStateCount++;
    }
    delay(100);
  }
  
  if (initialTiltStateCount < 25)
  {
    Serial.println("searching first");
    searchingFirst = true;
  }
  else
  {
    Serial.println("launching first");
    searchingFirst = false;
  }
  
  //if after 5 seconds, the rover was horizontal, search first.
  if (searchingFirst)
  {
    //CenterLogic();
    SendPequodMessage("Move Foward logic");
    MovingForwardLogic();
    SendPequodMessage("BaseAlign logic");
    BaseAlignLogic();
  }
  else
  {
    
  }
}

void CenterLogic()
{
  bool isCentered = false;
  
  while (!isCentered)
  {
    UltrasonicCheck();
    
    SendPequodMessage("LeftDistance");
    SendPequodMessage(distanceL);
    SendPequodMessage("RightDistance");
    SendPequodMessage(distanceR);
    
    if (abs((distanceL/distanceR) - 1) < 0.15 && distanceF > 20)
    {
      Serial.println("I am centered");
      isCentered = true;
    }
    else
    {
      stepperDirection = 'r';
    }
    
    MoveSteppers('t', false);
  }
  
  stepperDirection = 'f';
}

void MovingForwardLogic()
{
  stepperDirection = 'f';
  //UltrasonicCheck();
  bool isBaseFound = false;
  bool isTurnedToBase = false;
  double prevLeftDistance = distanceL;
  double prevRightDistance = distanceR;
  
  MoveSteppers('f', false);
  SendPequodMessage("Starting moving forward logic");
  
  if (isBaseFound)
  {
    SendPequodMessage("isBaseFound is true");
  }
  else
  {
    SendPequodMessage("isBaseFound is false");
  }
  

  
  if (isTurnedToBase)
  {
    SendPequodMessage("isTurnedToBase is true");
  }
  else
  {
    SendPequodMessage("isTurnedToBase is false");
  }
  
  while(!isBaseFound)
  {
    //UltrasonicCheck();
    
    SendPequodMessage("Front dist");
    SendPequodMessage(sonarF.ping_cm());
    SendPequodMessage("Left dist");
    SendPequodMessage(sonarL.ping_cm());
    SendPequodMessage("Right dist");
    SendPequodMessage(sonarR.ping_cm());
    
    if (sonarF.ping_cm() < 10 || isTurnedToBase)
    {
      Serial.println("I see the base in the front");
      SendPequodMessage("I see the base in the front");
      isBaseFound = true;
    }
    else if (prevLeftDistance - distanceL > 40)
    {
      Serial.println("I see the base to he left");
      SendPequodMessage("I see the base in the left");
      MoveABitFoward();
      Turn90Degrees('l');
      isTurnedToBase = true;
    }
    else if (prevRightDistance - distanceR > 40)
    {
      Serial.println("I see the base to the right");
      SendPequodMessage("I see the base in the right");
      MoveABitFoward();
      Turn90Degrees('r');
      isTurnedToBase = true;
    }
    else
    {
      stepperDirection = 'f';
    }
    
    MoveSteppers('f', false);
  }
}

void MoveABitFoward()
{
  stepperDirection = 'f';
  
  for (int x = 0; x < 2; x++)
  {
    MoveSteppers('f', false);
  }
}

void Turn90Degrees(char dir)
{
  stepperDirection = dir;
  SetStepperDirection();
  
  
  for (int x = 0; x < 130; x++)
  {
    digitalWrite(stepRPin, HIGH);
    digitalWrite(stepLPin, HIGH);

    delayMicroseconds(6000);

    digitalWrite(stepRPin, LOW);
    digitalWrite(stepLPin, LOW);
      
    delayMicroseconds(6000);
  }
}

//I am now facing the base, find me the middle of it
void BaseAlignLogic()
{
  Turn90Degrees('r');
  stepperDirection = 'b';
  SetStepperDirection();
  
  //UltrasonicCheck();
  //long leftSideStepCount = 0;
  long sideStepCount = 0;
  
  double prevDistance = distanceL;
  while (distanceL - prevDistance < 20)
  {
    digitalWrite(stepRPin, HIGH);
    digitalWrite(stepLPin, HIGH);

    delayMicroseconds(6000);

    digitalWrite(stepRPin, LOW);
    digitalWrite(stepLPin, LOW);
      
    delayMicroseconds(6000);
    //leftSideStepCount++;
  }
  
  stepperDirection = 'f';
  SetStepperDirection();
  
  /*
  for (int i=0; i < leftSideStepCount; i++)
  {
    digitalWrite(stepRPin, HIGH);
    digitalWrite(stepLPin, HIGH);

    delayMicroseconds(6000);

    digitalWrite(stepRPin, LOW);
    digitalWrite(stepLPin, LOW);
      
    delayMicroseconds(6000);
  }
  */
  
  while (distanceL - prevDistance < 20)
  {
    digitalWrite(stepRPin, HIGH);
    digitalWrite(stepLPin, HIGH);

    delayMicroseconds(6000);

    digitalWrite(stepRPin, LOW);
    digitalWrite(stepLPin, LOW);
      
    delayMicroseconds(6000);
    sideStepCount++;
  }

  stepperDirection = 'b';
  SetStepperDirection();
  
  for(int i = 0; i < sideStepCount/2; i++)
  {
    digitalWrite(stepRPin, HIGH);
    digitalWrite(stepLPin, HIGH);

    delayMicroseconds(6000);

    digitalWrite(stepRPin, LOW);
    digitalWrite(stepLPin, LOW);
      
    delayMicroseconds(6000);
  }
  
  Turn90Degrees('l');
}

void UltrasonicCheck()
{
  /*
  int checkRep = 5;
  long temp;
  
  //sonar.ping_cm()
  
  distanceL = 0;
  distanceR = 0;
  distanceF = 0;
  int divideL = 0;
  int divideR = 0;
  int divideF = 0;
  
  
  for (int x= 0; x < checkRep; x++)
  {
    //if out of range, drop the value
    temp = sonarL.ping_cm();
    if (temp != 0)
    {
      distanceL += temp;
      divideL++;
    }
    temp = sonarR.ping_cm();
    if (temp != 0)
    {
      distanceR += temp;
      divideR++;
    }
    temp = sonarF.ping_cm();
    if (temp != 0)
    {
      distanceF += temp;
      divideF++;
    }
  }
  
  if (divideL != 0)
  {
    distanceL /= divideL;
  }
  else
  {
    distanceL = 10000;
  }
  
  if (divideR != 0)
  {
    distanceR /= divideR;
  }
  else
  {
    distanceR = 10000;
  }
  if (divideF != 0)
  {
    distanceF /= divideF;
  }
  else
  {
    distanceF = 10000;
  }
  */
  
  distanceL = sonarL.ping_cm();
  distanceR = sonarR.ping_cm();
  distanceF = sonarF.ping_cm();
}

void MoveStraightForward(int numOfSteps)
{
  for (int x = 0; x < numOfSteps; x++)
  {
    
  }
}


void MoveSteppers(char movement, bool selfAlignFlag)
{
  //if align flag is on, try to swerve to center
  if (selfAlignFlag)
  {
    //if I am about centered, go both at half stepping
    if (abs((distanceL/distanceR) - 1) < 0.15)
    {
      digitalWrite(m0RPin, HIGH);
      digitalWrite(m1RPin, LOW);  
      digitalWrite(m0LPin, HIGH);
      digitalWrite(m1LPin, LOW);
    }
    //If I am cloer to right side, turn left my increasing the microstepping on left side
    //increasing the stepping will slow down the motor
    else if (distanceL > distanceR)
    {
      digitalWrite(m0RPin, HIGH);
      digitalWrite(m1RPin, LOW);  
      digitalWrite(m0LPin, LOW);
      digitalWrite(m1LPin, HIGH);
    }
    //vice versa
    else if (distanceL < distanceR)
    {
      digitalWrite(m0RPin, LOW);
      digitalWrite(m1RPin, HIGH);  
      digitalWrite(m0LPin, HIGH);
      digitalWrite(m1LPin, LOW);
    }
  }
  else
  {
    digitalWrite(m0RPin, HIGH);
    digitalWrite(m1RPin, LOW);  
    digitalWrite(m0LPin, HIGH);
    digitalWrite(m1LPin, LOW);
  }
  
  //true if not stop
  if (SetStepperDirection())
  {
    if (movement == 'f')
    {
      for (int x = 0; x < 80; x++)
      {
        digitalWrite(stepRPin, HIGH);
        digitalWrite(stepLPin, HIGH);

        delayMicroseconds(6000);

        digitalWrite(stepRPin, LOW);
        digitalWrite(stepLPin, LOW);
      
        delayMicroseconds(6000);
      }
    }
    else
    {
      for (int x = 0; x < 40; x++)
      {
        digitalWrite(stepRPin, HIGH);
        digitalWrite(stepLPin, HIGH);

        delayMicroseconds(6000);

        digitalWrite(stepRPin, LOW);
        digitalWrite(stepLPin, LOW);
      
        delayMicroseconds(6000);
      }
    }
  }
}

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
      digitalWrite(dirRPin, HIGH);
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

void SendPequodMessage(int number)
{
  String tempS = String(number);
  char tempC [10];

  tempS.toCharArray(tempC, 10);
  SendPequodMessage(tempC);
}

void SendPequodMessage(char *msg)
{  
  vw_send((uint8_t *)msg, strlen(msg));
  vw_wait_tx();
  delay(500);
}
