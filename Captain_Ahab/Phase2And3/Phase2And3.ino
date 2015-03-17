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

#include <SPI.h>
#include <Pixy.h>

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

char stepperDirection = 's'; //s, f, l, r, b



void setup() 
{
  // put your setup code here, to run once:
  
  Serial.begin(9600);
  Serial.println("Setup begin");
  
  pixy.init();
  
  StepperSetup();
  
  Serial.println("Setup finished");
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

void loop() 
{
  // put your main code here, to run repeatedly:

  //static int i = 0;
  
  PixySearch();
  
  
  
  MoveSteppers();
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
      
      for (int i = 0; i < blocks; i++)
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
          sig1X += (tempX- 160);
          sig1Y += pixy.blocks[i].y;
          sig1Count++;
        }
        else if (pixy.blocks[i].signature == 2)
        {
          long tempX = pixy.blocks[i].x;
          sig2X += (tempX- 160);
          sig2Y += pixy.blocks[i].y;
          sig2Count++;
        }
      }
    }
  }
  
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
    stepperDirection = 's';
  }
  
  Serial.println(stepperDirection);
}

void MoveSteppers()
{
  //static bool pulseHigh = 0;
  //static unsigned long timeStamp = 0;
  
  //PT_BEGIN(pt);
  
  //every half pulse, change direction and provide PWM
  //PT_WAIT_UNTIL(pt, micros() - timeStamp > stepperSpeed);
  
  //true if not stop
  if (SetStepperDirection())
  {
    for (int x = 0; x < 10; x++)
    {
      digitalWrite(stepRPin, HIGH);
      digitalWrite(stepLPin, HIGH);

      delay(4);

      digitalWrite(stepRPin, LOW);
      digitalWrite(stepLPin, LOW);
      
      delay(4);

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
