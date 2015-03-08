//http://harteware.blogspot.ca/2010/11/protothread-powerfull-library.html
//http://harteware.blogspot.ca/2010/11/protothread-and-arduino-first-easy.html
#include <pt.h>

static struct pt pt1, pt2;

int globalCount = 0;
int interval = 2000; //every 2 seconds
int initialWait = 1000;
void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  //Intialization
  PT_INIT(&pt1);
  PT_INIT(&pt2);
}

void loop() 
{
  // put your main code here, to run repeatedly:
  protoThread1(&pt1);
  //delay(1000);
  protoThread2(&pt2);
}

static int protoThread1(struct pt *pt)
{
  static unsigned long timestamp = 0;
  PT_BEGIN(pt);
  while(true)
  {
    PT_WAIT_UNTIL(pt, millis() - timestamp > interval);
    timestamp = millis();
    Serial.print("PT1: ");
    Serial.println(globalCount);
    globalCount++;
  }
  PT_END(pt);
}

static int protoThread2(struct pt *pt)
{
  static unsigned long timestamp = millis();
  PT_BEGIN(pt);
  
  PT_WAIT_UNTIL(pt, millis() - timestamp > initialWait);
  
  while(true)
  {
    PT_WAIT_UNTIL(pt, millis() - timestamp > interval);
    timestamp = millis();
    Serial.print("PT2: ");
    Serial.println(globalCount);
    globalCount++;
  }
  PT_END(pt);
}
