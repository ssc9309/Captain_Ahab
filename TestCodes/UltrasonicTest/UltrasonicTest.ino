#include <NewPing.h>

int trigPin = 12;
int echoPin = 11;
int maxDistance = 700; //maximum distance you want to see in cm
long duration, distance;

NewPing sonar(trigPin, echoPin, maxDistance);

void setup() 
{
  // put your setup code here, to run once:

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  /*
  //TestPins();
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH); //default timeout 1 sec
  
  if (duration == 0)
  {
    Serial.println("No feedback");
  }
  else
  {
    distance = duration/58.2;
    Serial.println(distance);
    delay(100);
  } 
  */
  
  unsigned int uS = sonar.ping();
  Serial.print("Ping: ");
  Serial.print(sonar.convert_cm(uS));
  Serial.println(" cm");
  delay(1000);
  
}

void TestPins()
{
  digitalWrite(trigPin, HIGH);
  digitalWrite(echoPin, HIGH);
  delay(1000);
  digitalWrite(trigPin, LOW);
  digitalWrite(echoPin, LOW);
  delay(1000);
}
