//Pixy notes:
//check the white balance with the laptop.
//make sure things are more white than yellow.
//ceiling will be our standard white balance for now.
// top left is x = y = 0

/*
pixy.blocks[i].signature The signature number of the detected object (1-7)
pixy.blocks[i].x The x location of the center of the detected object (0 to 319)
pixy.blocks[i].y The y location of the center of the detected object (0 to 199)
pixy.blocks[i].width The width of the detected object (1 to 320)
pixy.blocks[i].height The height of the detected object (1 to 200)
pixy.blocks[i].angle The angle of the object detected object if the detected object is a color code.
pixy.blocks[i].print() A member function that prints the detected object information to the serial port
*/

/*
//m0 low and m1 low mean Full step.
//m0 high and m1 low mean Half step.
//m0 high and m1 high mean 1/16th step.
//m0 low and ml high mean 1/8th step
*/

#include <SPI.h>
#include <Pixy.h>
#include <NewPing.h>
#include <VirtualWire.h>


bool turningPhase = false;
bool legoFound = false;
bool legoCentered = false;
bool baseFound = false;
bool centerSpinFinished = false;
long centerSpinStartTime = 0;
bool searchFinished = false;



bool approachingPhase = false;
int leftDistanceArray[10];
int rightDistanceArray[10];


//Stepper Pin
int m0RPin = 11;
int m1RPin = 10;
int stepRPin = 9;
int dirRPin = 8;

int m0LPin = 4;
int m1LPin = 5;
int stepLPin = 6;
int dirLPin = 7;



Pixy pixy;

char stepperDirection = 'r'; //s, f, l, r, b


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

double distanceR = -1;
double distanceL = -1;
double distanceF = -1;

//Transmitter Setep
int transmitterPin = 2;


void setup() 
{
  // put your setup code here, to run once:
  
  //Serial.begin(9600);
  //Serial.println("Setup begin");
  
  pixy.init();
  
  StepperSetup();
  UltrasonicSetup();
  TransmitterSetup();
  DistanceDataSetup();
  //Serial.println("Setup finished");
  
  //turningPhase = false;
  //approachingPhase = false;
  
  turningPhase = false;
  legoFound = false;
  legoCentered = false;
  baseFound = false;
  centerSpinFinished = false;
  centerSpinStartTime = 0;
  searchFinished = false;



  approachingPhase = false;
  
  
  digitalWrite(m0RPin, LOW);
  digitalWrite(m1RPin, LOW);
  digitalWrite(dirRPin, LOW);
  
  digitalWrite(m0LPin, LOW);
  digitalWrite(m1LPin, LOW);
  digitalWrite(dirLPin, HIGH);
  
  SendPequodMessage("Setup Finished");
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

void DistanceDataSetup()
{
  
  for (int x = 0; x < sizeof(leftDistanceArray); x++)
  {
    leftDistanceArray[x] = -1;
    rightDistanceArray[x] = -1;
  }
  
}

void loop() 
{
  // put your main code here, to run repeatedly:
  //SendPequodMessage("Looping...");
  
  //turningPhase = false;
  //approachingPhase = false;
  /*
  if (turningPhase)
  {
    SendPequodMessage("Turning Phase is on");
  }
  else
  {
    SendPequodMessage("Turning Phase is off");
  }
  
  if (approachingPhase)
  {
    SendPequodMessage("Approaching Phase is on");
  }
  else
  {
    SendPequodMessage("Approaching Phase is off");
  }
  */
  if (!turningPhase && !approachingPhase)
  {
    turningPhase = true;
    //exit the searching stage when
    //- lego found
    //- base found
    //- searched the open area, but no base or lego
    //  - center and face open area.
    //  - check open area.
    
    stepperDirection = 'r';
    
    SendPequodMessage("Turning Phase Start");
    
    //Hank, I changed my mind. I just want to center myself and find the base using ultrasonic.
    while(turningPhase)
    {
      //the ultrasonic will check the distance and set the direction it should go.
      UltrasonicCheck();
      //however, if the camera sees the objects, it will override it.
      //PixySearch();
      
      /*
      //if lego is found in the camera, the PixySearch has already set the direction
      if (legoFound)
      {
        SendPequodMessage("Lego Found");
        searchingPhase = false;
        approachingPhase = true;
        break;
      }
      //if the base is found, direction is set also by PixySearch. 
      //if I am too close to the base, I must see the lego anyway
      else if (baseFound)
      {
        SendPequodMessage("Base Found");
        searchingPhase = false;
        approachingPhase = true;
        break;
        
        //hank, this also goes to the approaching phase.
        
        //if (distanceF <= 4)
        //{
          //I shouldn't be here.
          //if the base is this close, I have to see the lego.
        //}
        
      }
      else
      {
        //if the left and right is about 10-20 percent difference, and the front distance > 20.
        //you are facing correctly
        //hank. test for the distance sensing for angled wall.
        
        
        if (abs(distanceL/distanceR) - 1 < 0.2 && distanceF > 10)
        {
//          String temp = String()
          SendPequodMessage("I am centered");
          centerSpinFinished = true;
        }
        
        //If i am not centered to the field B yet, turn.
        if (!centerSpinFinished)
        {
          stepperDirection = 'r';
      
          if (centerSpinStartTime == 0)
          {
            centerSpinStartTime = millis();
          }
          else
          {
            //i have been spinning for more than 10 seconds.
            //give up.
            if (millis() - centerSpinStartTime > 10000)
            {
              //
              //hank. check the steps if I have turned more than 360 degrees in the centerSpin stage.
              //If I have done so, 
            }
          }
          
        }
        //If I am centered, go forward.
        else
        {
          SendPequodMessage("Center Spin finished");
          
          stepperDirection = 'f';
          
          searchingPhase = false;
          approachingPhase = true;
          break;
          //try to go forward in the centre of the field b.
          //compare left and right distances.
        }
      }
      */
      
      String tempS = String(distanceL);
      char tempC [10];
      /*
      
      tempS.toCharArray(tempC, 10);
      SendPequodMessage("Left sonar");
      SendPequodMessage(tempC);
      */
      
      tempS = String(distanceL);
      tempS.toCharArray(tempC, 10);
      SendPequodMessage("Left sonar");
      SendPequodMessage(tempC);
      
      
      tempS = String(distanceF);
      tempS.toCharArray(tempC, 10);
      SendPequodMessage("Forward sonar");
      SendPequodMessage(tempC);
      
      
      tempS = String(distanceR);
      tempS.toCharArray(tempC, 10);
      SendPequodMessage("Right sonar");
      SendPequodMessage(tempC);
      
      //SendPequodMessage(distanceL);
      
      
      if (abs((distanceL/distanceR) - 1) < 0.15 && distanceF > 20)
      {
//          String temp = String()
        SendPequodMessage("I am centered");
        centerSpinFinished = true;
      }
      
      if (!centerSpinFinished)
      {
        stepperDirection = 'r';
      
        if (centerSpinStartTime == 0)
        {
          centerSpinStartTime = millis();
        }
        else
        {
            //i have been spinning for more than 10 seconds.
            //give up.
          if (millis() - centerSpinStartTime > 10000)
          {
            SendPequodMessage("I've been turning for more than 10 seconds");
              //
              //hank. check the steps if I have turned more than 360 degrees in the centerSpin stage.
              //If I have done so, 
          }
        }
      }
        //If I am centered, go forward.
      else
      {
        SendPequodMessage("Center Spin finished");
          
        stepperDirection = 'f';
          
        turningPhase = false;
        approachingPhase = true;
        break;
        //try to go forward in the centre of the field b.
        //compare left and right distances.
      }
      
      
      
      
      MoveSteppers('t', false);
    }
    
    SendPequodMessage("Searching Phase End");
    
    turningPhase = false;
    approachingPhase = true;
  }
  
  //I am now in the approaching phase.
  if (!turningPhase && approachingPhase)
  {
    SendPequodMessage("Approaching Phase start");
    
    while(approachingPhase)
    {
      //the ultrasonic will check the distance and set the direction it should go.
      UltrasonicCheck();
      //however, if the camera sees the objects, it will override it.
      PixySearch();
      
      //if searchingPhase found the lego, chase the lego and capture
      if (legoFound)
      {
        SendPequodMessage("Following Lego");
        //but, if the lego is found and distance <= 4 cm, stop and fire
        if (legoCentered && distanceF <= 4)
        {
          SendPequodMessage("Centering for Lego");
          //aim for to get ready to fire first.
          //aim logic.
          //look at the lego.
          //turn left and right to the edges.
          //see which edge is closer.
          //move away from the closer edge to the center.
          //face the lego again.
          //then fire.
          
          int prevDistance = distanceF;
          int leftCount = 0;
          int rightCount = 0;
          
          //turn left until you don't see the base anymore.
          stepperDirection = 'l';
          while (abs(prevDistance - distanceF) > 10)
          {
            MoveSteppers('t', false);
            UltrasonicCheck();
            leftCount++;
          }
          
          //after seeing the left edge of the base, turn back to center
          stepperDirection = 'r';
          for (int x = 0; x < leftCount; x++)
          {
            MoveSteppers('t', false);
          }
          
          //Turn right until you don't see the base anymore
          prevDistance = distanceF;
          while (abs(prevDistance - distanceF) > 10)
          {
            MoveSteppers('t', false);
            UltrasonicCheck();
            rightCount++;
          }
          
          //stop once you are at the edge of the right side.
          stepperDirection = 's';
          
          SendPequodMessage("I should be facing right side of base");
          
          //current placeholder to stop it from doing anything
          approachingPhase = false;
          turningPhase = true;     
          SendPequodMessage("End of approacing phase.");
          //stepperDirection = 's';
          
          //fire
          //searchingPhase = false;
        }
      }
      //if searchingPhase found the base, move towards base, find lego, and capture.
      else if(baseFound)
      {
        SendPequodMessage("Following Lego");
      }
      //I have not seen base or lego out of the 360 turn, I need to search as moving.
      //I am on the hunt for base(rapid distance change) or lego
      else
      {
        SendPequodMessage("Looking around");
        stepperDirection = 'f';
        
        //selfAlignFlag = true;
        
        //if the base has been detected on a side
        char sideLetter = BlindSideBaseCheck(distanceL, distanceR);
        
        String tmpS = String(sideLetter);
        char tmpC[10];
        tmpS.toCharArray(tmpC, 10);
        SendPequodMessage(tmpC);
        
        if (sideLetter != 'n') 
        {
          SendPequodMessage("Found a base to the side");
          //move forward to align yourself to the middle of the base
          for (int x = 0; x < 4; x++)
          {
            MoveSteppers('f', false);
          }
          
          SendPequodMessage("Moved to middle. Turning now.");
          
          
          /*
          tempS = String(distanceL);
      tempS.toCharArray(tempC, 10);
      SendPequodMessage("Left sonar");
      SendPequodMessage(tempC);
          */
          
          stepperDirection = sideLetter;
          
          //this better be a 90 degree turn
          for (int x = 0; x < 4; x++)
          {
            MoveSteppers('t', false);
          }
          
          
          
          //move forward until camera sees the lego
          
          while(!legoFound)
          {
            PixySearch();
            stepperDirection = 'f';
            MoveSteppers('f', false);
          }
          
          SendPequodMessage("I see the lego");
        }
      }
      
      MoveSteppers('f', true);
    }
    
    SendPequodMessage("Approachig Phase end");
  }
  //PixySearch();
  //UltrasonicCheck();
  
  
  //MoveSteppers();
}

//will return 'l' (left), 'r' (right), 'n' (none)


int prevLeftDistance = -1;
int prevRightDistance = -1;

char BlindSideBaseCheck(int distanceL, int distanceR)
{
  if (prevLeftDistance == -1)
  {
    SendPequodMessage("Initializing blind side logic");
    prevLeftDistance = distanceL;
    prevRightDistance = distanceR;
  }
  else
  {
    SendPequodMessage("Calculating Blind side ");
    if (abs(prevLeftDistance - distanceL) > 40)
    {
      SendPequodMessage("bs: turn left");
      return 'l';
    }
    else if (abs(prevRightDistance - distanceR) > 40)
    {
      SendPequodMessage("bs: turn right");
      return 'r';
    }
    else
    {
      SendPequodMessage("bs: don't turn");
      prevRightDistance = distanceR;
      prevLeftDistance = distanceL;
      
      return 'n';
    }
  }
  
  return 'n';
  SendPequodMessage("bs: how am i here?");
  
  
  /*
  for (int i = 0; i < sizeof(leftDistanceArray); i++)
  {
    if (leftDistanceArray[i] == -1)
    {
      leftDistanceArray[i] = distanceL;
      rightDistanceArray[i] = distanceR;
      break;
    }
  }
  
  if (sizeof(leftDistanceArray) > 1)
  {
    int prevLeftDistance = -1;
    int currentLeftDistance = -1;
    int prevRightDistance = -1;
    int currentRightDistance = -1;
    
    for (int i = 0; i < sizeof(leftDistanceArray); i++)
    {
      currentLeftDistance = leftDistanceArray[i];
      currentRightDistance = rightDistanceArray[i];
      
      if (prevLeftDistance == -1)
      {
        prevLeftDistance = currentLeftDistance;
        prevRightDistance = currentRightDistance;
      }
      else
      {
        if (abs(currentLeftDistance - prevLeftDistance) > 40)
        {

          //base detected on left side
          return 'l';
          /*
          //void MoveSteppers(char movement, bool selfAlignFlag)
          for (int x = 0; x < 4; x++)
          {
            MoveSteppers('f', false);
          }
          stepperDirection = "l";
          for (int x = 0; x < 4; x++)
          {
            MoveSteppers('t', false);
          }
          */
          /*
        }
        
        else if (abs(currentRightDistance - prevRightDistance) > 40)
        {
          //base detected on right side
          return 'r';
          /*
          for (int x = 0; x < 4; x++)
          {
            MoveSteppers('f', false);
          }
          stepperDirection = "r";
          for (int x = 0; x < 4; x++)
          {
            MoveSteppers('t', false);
          }
          */
          /*
        }
      }
    }
    
    return 'n';
  }
  */
}

void PixySearch()
{
  long blocks;
  
  //0 for sig X will be center of the frame => x = 160
  //- is left of the frame, + is right of the frame
  //y is not centered to 100 yet
  long sig1X = 0;
  long sig1Y = 0;
  long sig1Count = 0;
  bool sig1Found = false;
  
  long sig2X = 0;
  long sig2Y = 0;
  long sig2Count = 0;  
  
  //take 10 images and average
  //hank: this doesn't work because the cam is only at 50 Hz. Only takes 1 picture anyway. Derp.
  for (int imageCount = 0; imageCount < 10; imageCount++)
  {
    blocks = pixy.getBlocks();
  
    if (blocks > 0)
    {
    //i++;
    
    
    //Serial.print("Num of blocks: ");
    //Serial.println(blocks);
      SendPequodMessage("I see blocks");
      for (int i = 0; i < blocks; i++)
      {
        //filter out anything top quarter of the screen (y = 0 to 50)
        //hank, change this value once rover is done.
        if (pixy.blocks[i].y > 50)
        {
          if (pixy.blocks[i].signature == 1)
          {
            sig1Found = true;

            long tempX = pixy.blocks[i].x;
          //Serial.println("Raw x value");
          //Serial.println(pixy.blocks[i].x);
          //Serial.println(pixy.blocks[i].x - 160);
          //Serial.println(tempX);
          //Serial.println(tempX- 160);
            sig1X += (tempX - 160);
            sig1Y += pixy.blocks[i].y;
            sig1Count++;
          }
          else if (pixy.blocks[i].signature == 2)
          {
            //baseFound = true;
            
            //if the block is too high (above half line), filter it out.
            
            long tempX = pixy.blocks[i].x;
            long tempY = pixy.blocks[i].y;
            long tempH = pixy.blocks[i].height;
            
            if(tempY + (tempH/2) > 70)
            {
              sig2X += (tempX - 160);
              sig2Y += pixy.blocks[i].y;
              sig2Count++;
            }
          }
        }
      }
    }
  }
  
  //The following two will average the sum of X and Y signals
  if (sig1Count > 0)
  {
    //Serial.println("Before process");
    //Serial.print(sig1X);
    //Serial.print(", ");
    //Serial.println(sig1Count);
    sig1X /= sig1Count;
    sig1Y /= sig1Count;
  }
  if (sig2Count > 0)
  {
    sig2X /= sig2Count;
    sig2Y /= sig2Count;
  }
    
    
    //x center is 160
    
  //Serial.print("Center: ");
  //Serial.print(sig1X);
  //Serial.print(", ");
  //Serial.println(sig1Y);
  
  //set stepper direction depending on the cam observation
  //if nothing is found, keep doing whatever you were doing before
  if (sig1Count == 0 && sig2Count == 0)
  {
    legoFound = false;
    baseFound = false;
    //stepperDirection = 'r';
  }
  //if the lego has been located
  else if (sig1Count > 0)
  {
    legoFound = true;
    
    if (sig1X > 20)
    {
      stepperDirection = 'r';
    }
    else if (sig1X < -20)
    {
      stepperDirection = 'l';
    }
    else
    {
      legoCentered = true;
      stepperDirection = 'f';
    }
  }
  else if (sig2Count > 0)
  {
    //if the distance is too close but have not seen lego, this is not base
    //hank I don't need ths filter. Ben's height filter will be sufficient... i think.
    
    //if (distanceF > 20)
    //{
      baseFound = true;
      if (sig2X > 20)
      {
        stepperDirection = 'r';
      }
      else if (sig2X < -20)
      {
        stepperDirection = 'l';
      }
      else
      {
        stepperDirection = 'f';
      }
    //}
  }
  
  //Serial.println(stepperDirection);
}

//movement will be either 't' (turning) or 'f' (forward)
//when forward, move more
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

  
  //static bool pulseHigh = 0;
  //static unsigned long timeStamp = 0;
  
  //PT_BEGIN(pt);
  
  //every half pulse, change direction and provide PWM
  //PT_WAIT_UNTIL(pt, micros() - timeStamp > stepperSpeed);
  
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
  //if (sonar.convert_cm(sonar.ping()) < 10)
  //{
  //  stepperDirection = 's';
  //}
  //Serial.println(stepperDirection);
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

//change the sonarR, L, F depending on the current setup.
void UltrasonicCheck()
{
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
    //distanceL += sonarL.ping_cm();
    //distanceR += sonarR.ping_cm();
    //distanceF += sonarF.ping_cm();
  }
  
  if (divideL != 0)
  {
    distanceL /= divideL;
  }
  if (divideR != 0)
  {
    distanceR /= divideR;
  }
  if (divideF != 0)
  {
    distanceF /= divideF;
  }
}

void SendPequodMessage(char *msg)
{  
  vw_send((uint8_t *)msg, strlen(msg));
  vw_wait_tx();
  delay(100);
}
