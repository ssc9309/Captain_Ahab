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

//Threading
//pt1 = motordrive
static struct pt pt1, pt2, pt3;
int stepperSpeed = 0; //in HZ up to 1500
char stepperDirection = 's'; //s = stop, f = forward, r = right, l = left, b = back


void setup() 
{
  // put your setup code here, to run once:
  
  Serial.begin(9600);
  
  
  ThreadingSetup();
}

void ThreadingSetup()
{
  PT_INIT(&pt1);
  PT_INIT(&pt2);
  PT_INIT(&pt3);
}

void loop() 
{
  // put your main code here, to run repeatedly:
  AhabMovementThread(&pt1);
}

static int AhabMovementThread(struct pt *pt)
{
  PT_BEGIN(pt);
  
  PT_END(pt);
}
