#include <Wire.h>
#include <PVision.h>

PVision greenCam;

byte greenCamResult;


void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  Serial.println("Setup");
  greenCam.init();
  Serial.println("Setup end");
}

void loop() 
{
  // put your main code here, to run repeatedly:
  greenCamResult = greenCam.read();
  
  Serial.print(millis());
  Serial.print(": greenCamResult: ");
  Serial.println(greenCamResult);
  
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
  
  delay(1000);
}
