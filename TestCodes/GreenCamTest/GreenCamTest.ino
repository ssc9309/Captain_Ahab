#include <Wire.h>
#include <PVision.h>

PVision greenCam;

byte greenCamResult;
int iterationCount = 0;

void setup() {
  // put your setup code here, to run once:
  //Wire.begin();
  Serial.begin(9600);
  Serial.println("Setup");
  greenCam.init();
  Serial.println("Setup end");
}

void loop() 
{
  // put your main code here, to run repeatedly:
  greenCamResult = greenCam.read();
  iterationCount++;
  
  Serial.print(millis());
  Serial.print(": greenCamResult: ");
  Serial.println(greenCamResult);
  
  if (greenCam.Blob1.Size > 0 && greenCam.Blob1.Size < 15)
  {
    Serial.println("BLOB1");
    Serial.print("X: ");
    Serial.println(greenCam.Blob1.X);
    Serial.print("Y: ");
    Serial.println(greenCam.Blob1.Y);
    Serial.print("Size: ");
    Serial.println(greenCam.Blob1.Size);
  }
  if (greenCam.Blob2.Size > 0 && greenCam.Blob2.Size < 15)
  {
    Serial.println("BLOB2");
    Serial.print("X: ");
    Serial.println(greenCam.Blob2.X);
    Serial.print("Y: ");
    Serial.println(greenCam.Blob2.Y);
    Serial.print("Size: ");
    Serial.println(greenCam.Blob2.Size);
  }
  if (greenCam.Blob3.Size > 0 && greenCam.Blob3.Size < 15)
  {
    Serial.println("BLOB3");
    Serial.print("X: ");
    Serial.println(greenCam.Blob3.X);
    Serial.print("Y: ");
    Serial.println(greenCam.Blob3.Y);
    Serial.print("Size: ");
    Serial.println(greenCam.Blob3.Size);
  }
  if (greenCam.Blob4.Size > 0 && greenCam.Blob4.Size < 15)
  {
    Serial.println("BLOB4");
    Serial.print("X: ");
    Serial.println(greenCam.Blob4.X);
    Serial.print("Y: ");
    Serial.println(greenCam.Blob4.Y);
    Serial.print("Size: ");
    Serial.println(greenCam.Blob4.Size);
  }
  
  if (millis()/1000 == 10)
  {
    Serial.print("Iteration over 10 seconds: ");
    Serial.println(iterationCount);
  }
  //delay(1000);
}
