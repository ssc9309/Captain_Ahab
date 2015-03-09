//http://harteware.blogspot.ca/2010/11/protothread-powerfull-library.html
//http://harteware.blogspot.ca/2010/11/protothread-and-arduino-first-easy.html

//This is not truly a multithread
//It mimics by keep on calling the thread and checking the status of it.
//PT Wait function kills the method
//only static variable is kept in the method

#include <pt.h>

static struct pt pt1, pt2, pt3;

int globalCount = 0;
int threadCount = 0;
int interval = 2000; //every 2 seconds
int initialWait = 1000;
void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  //Intialization
  PT_INIT(&pt1);
  PT_INIT(&pt2);
  PT_INIT(&pt3);
}

void loop() 
{
  // put your main code here, to run repeatedly:
  protoThread1(&pt1, threadCount);
  //delay(1000);
  protoThread2(&pt2, threadCount);
  //threadCount++;
  //protoThread3(&pt3);
}

bool T1FirstCall = true;

static int protoThread1(struct pt *pt, int threadCount)
{
  static unsigned long timestamp = 0;
  
  PT_BEGIN(pt);
  
  while(true)
  {
    if (T1FirstCall)
    {
      Serial.print(millis());
      Serial.print(": PT1: ");
      Serial.println(globalCount);
//      Serial.print(": Threadcount: ");
//      Serial.println(threadCount);
      globalCount++;
      T1FirstCall = false;
    }
    else
    {
      PT_WAIT_UNTIL(pt, millis() - timestamp > interval);
      timestamp = millis();
      Serial.print(millis());
      Serial.print(": PT1: ");
      Serial.println(globalCount);
//      Serial.print(": Threadcount: ");
//      Serial.println(threadCount);
      globalCount++;
    }    
  }
  PT_END(pt);
}

bool T2FirstCall = true;

static int protoThread2(struct pt *pt, int threadCount)
{
  static unsigned long timestamp = 0;
  PT_BEGIN(pt);
  
  PT_WAIT_UNTIL(pt, millis() - timestamp > initialWait);
  
  while(true)
  {
    if (T2FirstCall)
    {
      timestamp = millis();
      Serial.print(millis());
      Serial.print(": PT2: ");
      Serial.println(globalCount);
//    Serial.print(": Threadcount: ");
//    Serial.println(threadCount);
      globalCount++;
      T2FirstCall = false;
    }
    else
    {
      PT_WAIT_UNTIL(pt, millis() - timestamp > interval);
      timestamp = millis();
      Serial.print(millis());
      Serial.print(": PT2: ");
      Serial.println(globalCount);
//    Serial.print(": Threadcount: ");
//    Serial.println(threadCount);
      globalCount++;
    }
    
    
  }
  PT_END(pt);
}

static int protoThread3(struct pt *pt)
{
  static unsigned long timestamp = 0;
  static int internalCount = 0;
  
  PT_BEGIN(pt);
  
  //while(true)
  //{
    PT_WAIT_UNTIL(pt, millis() - timestamp > interval);
    timestamp = millis();
    Serial.println("Thread3:");
    Serial.print("Millis: ");
    Serial.println(millis());
    Serial.print("Timestamp: ");
    Serial.println(timestamp);
    Serial.print("Internal Count: ");
    Serial.println(internalCount);
    internalCount++;
    Serial.print("GlobalCount: ");
    Serial.println(globalCount++);
  //}
  
  PT_END(pt);
}
