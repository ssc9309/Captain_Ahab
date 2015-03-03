#include <PVision.h>
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal.h>
//Test change

PVision greenCam;


int ledPin = 5;
byte greenCamResult;
int numOfBlobs = 0;


//there are 5 phases total
//1. Landing
//2. Searching
//3. Approaching
//4. Rescuing
//5. Recovering

boolean landingPhase = false;
boolean searchingPhase = false;
boolean approachingPhase = false;
boolean rescuingPhase = false;
boolean recoveringPhase = false;


LiquidCrystal lcd(7, 8, 9, 10, 11, 12);


void setup()
{
  lcd.begin(16, 2);
  
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  greenCam.init();
  
  //comment out the below if you don't want to test communication
  //TestCommunicationSetup();
}

void loop()
{
  //if all are false, program just started. Therefore landing phase.
  if(!landingPhase && !searchingPhase && !approachingPhase && !rescuingPhase && !recoveringPhase)
  {
    landingPhase = true;
    //tell Pequod to pull the pin
    //monitor accelerometer until stopped falling
    //or should I monitor the servo for impact then weight load?
    //tell Pequod to lower
    //monitor accelerometer or servo
    //complete the phase when reached the ground
    landingPhase = false;
    searchingPhase = true;
  }
  //if it is searching phase now
  else if (!landingPhase && searchingPhase && !approachingPhase && !rescuingPhase && !recoveringPhase)
  {
    //Counter Clockwise will be a positive turn
    int numOfLightFound = 0;
    int numOfDegreeTurned = 0;
    int lightAngle1 = 0;
    int lightAngle2 = 0;
    

    while(searchingPhase)
    {
      if(numOfDegreeTurned >= 360)
      {
        if (numOfLightFound == 0)
        {
          //move to a different location
          // aka give up
        }
        //if you have seen only 1 light in 360 degrees, turn to it
        //search phase over
        else
        {
          searchingPhase = false;
          TurnToLight(numOfLightFound, lightAngle1, lightAngle2);
        }
      }
      else
      {
        //true if facing a light
        if (AreYouFacingALight())
        {
          //if found the first light, record the angle
          if(numOfLightFound == 0)
          {
            numOfLightFound++;
            lightAngle1 = numOfDegreeTurned;
          }
          //if both lights are found, turn to it.
          //searching phase over
          else if (numOfLightFound == 1)
          {
            numOfLightFound++;
            lightAngle2 = numOfDegreeTurned;
            searchingPhase = false;
            
            TurnToLight(numOfLightFound, lightAngle1, lightAngle2);
          }
        }
      }
      
      //if the search is still on, keep turning
      if (searchingPhase)
      {
        //turn one degree.
      }
    }
    
    approachingPhase = true;
  }

  else if (!landingPhase && !searchingPhase && approachingPhase && !rescuingPhase && !recoveringPhase)
  {
    
  }
  //TestCommunication();
  //TestGreenCam();
  //TestLEDBlink();
}

//if two lights have been seen, turn to the centre of them
//if only one light has been found, turn directly to it.
void TurnToLight(int numOfLight, int lightAngle1, int lightAngle2)
{
  int angleToTurn = 0;
  
  //if only one light has been seen, turn directly to it
  //do enter this condition, assume that it has turned exactly 360 degrees
  //which it had to have btw.
  if(numOfLight == 1)
  {
    angleToTurn = 360 - lightAngle1;
    
    //if turning right is closer, then negative angle (clock wise turn = negative)
    if (angleToTurn < 180)
    {
      angleToTurn = angleToTurn * -1;
    }
  }
  
  //if you have seen 2 lights, that means you must be facing the 2nd light right now.
  //hank. The 2nd light may not be the left light of the two. What if you start from seeing the left light first?
  //Also speaking of which, what if you see the light after you land right away but on the right side?
  //need to change flow chart
  //if not, I am dumb
  else if (numOfLight == 2)
  {
    //calculate how much angle to turn to arrive that the mid point
    angleToTurn = lightAngle2 - lightAngle1;
    
    //it is negative as it will have to always clock wise after seeing the second light.
    angleToTurn = -angleToTurn / 2;
  }
  else
  {
    //if you are here, I don't know what to say.
  }
  
  //TurnTheRover(int angleToTurn)
}

void AreYouFacingALight()
{
  //use the green cam and check if I am centered to a light in the x axis.
  //you need to use sweet spot the tolerance to not miss the light but not overlap with the first one.
  
  //if facing a light
  //  return true
  
  return false;
}

void TestCommunicationSetup()
{
  //TX out of Arudino is Digital Pin 1
  
  //I think this is 4800. If doesn't work, use 2400
  Serial.begin(4800);
}

void TestCommunication()
{
  int counter = 0;
  int increment = 1;
  
  //counter goes up and down between 0 and 100
  //send the counter to the receiver
  //change counter every half second
  while(true)
  {
    if(counter == 0)
    {
      increment = 1;
    }
    else if (counter == 100)
    {
      increment = -1;
    }
    
    Serial.print(counter);
    counter += increment;
    
    delay(500);
  }
}

void TestGreenCam()
{
  //greenCamResult is a byte
  //if all are one the response is 00001111 = 15
  greenCamResult = greenCam.read();
  
  Serial.print(millis());
  Serial.print(" - greenCamResult: ");
  Serial.println(greenCamResult, DEC);
  
  //if any blobs are found
  if (greenCamResult > 0)
  {
    //numOfBlobs = CountBlobs();
    DisplayLocationsOfBlobs();
  }
  
  delay(250);
}

//This function will return the number of Blobs currently seen by the camera
//obsolete because greenCam.read() gives you the number of blobs
int CountBlobs()
{
  int count = 0;
  
  if (BLOB1)
  {
    count++;
  }
  if (BLOB2)
  {
    count++;
  }
  if (BLOB3)
  {
    count++;
  }
  if (BLOB4)
  {
    count++;
  }
  
  return count;
}

//This function will just display the x y coordination of the available blobs
void DisplayLocationsOfBlobs()
{
  if (greenCam.Blob1.Size > 0)
  {
    Serial.println("BLOB1");
    Serial.print("X: ");
    Serial.println(greenCam.Blob1.X);
    Serial.print("Y: ");
    Serial.println(greenCam.Blob1.Y);
  }
  if (greenCam.Blob2.Size > 0)
  {
    Serial.println("BLOB2");
    Serial.print("X: ");
    Serial.println(greenCam.Blob2.X);
    Serial.print("Y: ");
    Serial.println(greenCam.Blob2.Y);
  }
  if (greenCam.Blob3.Size > 0)
  {
    Serial.println("BLOB3");
    Serial.print("X: ");
    Serial.println(greenCam.Blob3.X);
    Serial.print("Y: ");
    Serial.println(greenCam.Blob3.Y);
  }
  if (greenCam.Blob4.Size > 0)
  {
    Serial.println("BLOB4");
    Serial.print("X: ");
    Serial.println(greenCam.Blob4.X);
    Serial.print("Y: ");
    Serial.println(greenCam.Blob4.Y);
  }
}

//This function will find the centre of lights and ask the motor to turn.
void FaceTheLego()
{
  int legoX = 0;
  if (numOfBlobs == 2)
  {
    legoX = greenCam.Blob1.X + greenCam.Blob2.X;
    legoX /= 2;
    
    //if legoX is not straight, turn
  }
  else if (numOfBlobs == 1)
  {
    legoX = greenCam.Blob1.X;
    
    //if legoX is not straight, turn
  }
}



void TestLEDBlink()
{
  digitalWrite(ledPin, HIGH);
  //set the cursor to column 0, line 1
  lcd.setCursor(0, 1);
  lcd.println("LED on");
  lcd.setCursor(0, 2);
  lcd.println("LED will be turning off soon");
  delay(1000);
  
  digitalWrite(ledPin, LOW);
  lcd.setCursor(0, 1);
  lcd.println("LED off");
  lcd.setCursor(0, 2);
  lcd.println("LED will be turning on soon");
  delay(1000);
}
