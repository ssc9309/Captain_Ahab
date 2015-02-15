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
}

void loop()
{
  /*
  greenCamResult = greenCam.read();
  
  //if any blobs are found
  if (greenCamResult)
  {
    numOfBlobs = CountBlobs();
    DisplayLocationsOfBlobs();
  }
  */
  
  TestLEDBlink();
}

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

void DisplayLocationsOfBlobs()
{
  if (BLOB1)
  {
    Serial.println("BLOB1");
    Serial.print("X: ");
    Serial.println(greenCam.Blob1.X);
    Serial.print("Y: ");
    Serial.println(greenCam.Blob1.Y);
  }
  if (BLOB2)
  {
    Serial.println("BLOB2");
    Serial.print("X: ");
    Serial.println(greenCam.Blob2.X);
    Serial.print("Y: ");
    Serial.println(greenCam.Blob2.Y);
  }
  if (BLOB3)
  {
    Serial.println("BLOB3");
    Serial.print("X: ");
    Serial.println(greenCam.Blob3.X);
    Serial.print("Y: ");
    Serial.println(greenCam.Blob3.Y);
  }
  if (BLOB4)
  {
    Serial.println("BLOB4");
    Serial.print("X: ");
    Serial.println(greenCam.Blob4.X);
    Serial.print("Y: ");
    Serial.println(greenCam.Blob4.Y);
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
