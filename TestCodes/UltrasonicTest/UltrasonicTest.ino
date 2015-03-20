#include <NewPing.h>
#include <VirtualWire.h>

int trig1Pin = A5;
int echo1Pin = A4;

int trig2Pin = A2;
int echo2Pin = A1;
int trig3Pin = 13;
int echo3Pin = 12;


int testTrigPin = 2;
int testEchoPin = 3;


int maxDistance = 700; //maximum distance you want to see in cm
long duration, distance;

int transPin = 2;


NewPing sonarL(trig1Pin, echo1Pin, maxDistance);
NewPing sonarR(trig2Pin, echo2Pin, maxDistance);
NewPing sonarF(trig3Pin, echo3Pin, maxDistance);

NewPing sonarTest(testTrigPin, testEchoPin, maxDistance);

void setup() 
{
  Serial.begin(9600);
  // put your setup code here, to run once:

  //pinMode(trig1Pin, OUTPUT);
  //pinMode(echo1Pin, INPUT);
  //pinMode(trig2Pin, OUTPUT);
  //pinMode(echo2Pin, INPUT);
  //pinMode(trig3Pin, OUTPUT);
  //pinMode(echo3Pin, INPUT);
  
  TransmitterSetup();
  
}


void TransmitterSetup()
{
  vw_set_ptt_inverted(true); // Required for DR3100
  vw_set_tx_pin(transPin);
  vw_setup(2000);	 // Bits per sec
  
  SendPequodMessage("Setup");
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
  
  /*
  unsigned int uS = sonar.ping();
  Serial.print("Ping: ");
  Serial.print(sonar.convert_cm(uS));
  Serial.println(" cm");
  */
  
  //Serial.print("Sonar1: ");
  //Serial.println(sonar1.ping_cm());
  //Serial.print("Sonar2: ");
  //Serial.println(sonar2.ping_cm());
  //Serial.print("Sonar3: ");
  //Serial.println(sonar3.ping_cm());
  //Serial.println();
  
  //Serial.print("SonarTest: ");
  //Serial.println(sonarTest.ping_cm());
  //Serial.println();
  
  int distanceL = sonarL.ping_cm();
  
  
  String tempS = String(distanceL);
  char tempC [10];

      
  tempS.toCharArray(tempC, 10);
  SendPequodMessage("Left sonar");
  SendPequodMessage(tempC);
  
  int distanceR = sonarR.ping_cm();
  
  tempS = String(distanceR);
  tempC [10];
    
  tempS.toCharArray(tempC, 10);
  SendPequodMessage("Right sonar");
  SendPequodMessage(tempC);

  int distanceF = sonarF.ping_cm();
  tempS = String(distanceF);
  tempC [10];
    
  tempS.toCharArray(tempC, 10);
  SendPequodMessage("Forward sonar");
  SendPequodMessage(tempC);
  
  //SendDataToPequod(String(sonar.convert_cm(uS)));
  //SendDataToPequod((char*)sonar.convert_cm(uS));
  //SendDataToPequod("Hello");
  
  delay(1000);
  
}

void SendPequodMessage(char *msg)
{
  Serial.print("Sending: ");
  Serial.println(msg);
  
  vw_send((uint8_t *)msg, strlen(msg));
  vw_wait_tx(); // Wait until the whole message is gone
  delay(100);
}

/*
void TestPins()
{
  digitalWrite(trigPin, HIGH);
  digitalWrite(echoPin, HIGH);
  delay(1000);
  digitalWrite(trigPin, LOW);
  digitalWrite(echoPin, LOW);
  delay(1000);
}
*/
