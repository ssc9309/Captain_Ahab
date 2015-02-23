#include <PVision.h>
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal.h>
//Test change

PVision greenCam;


int ledPin = 5;
byte greenCamResult;
int numOfBlobs = 0;

//Captain Ahab (rover) will be fired and lowered by Pequod (crane) during deployment phase.
//He will then s
boolean deploymentPhaseOver = false;
boolean scanPhaseOver = false;
boolean travelPhaseOver = false;
boolean retrievalPhaseOver = false;


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
  
  //TestCommunication();
  TestGreenCam();
  //TestLEDBlink();
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