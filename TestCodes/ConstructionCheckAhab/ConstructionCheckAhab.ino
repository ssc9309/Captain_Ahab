#include <VirtualWire.h>
#include <ServoTimer2.h>
#include <NewPing.h>


//UltraSonic Pins
int trigPin = 12;
int echoPin = 11;
int maxDistance = 700;

//Stepper Pins
//m0 low and m1 low mean Full step.
//m0 high and m1 low mean Half step.
//m0 high and m1 high mean 1/16th step.
int m0RPin = 4;
int m1RPin = 5;
int stepRPin = 2;
int dirRPin = 3;

int m0LPin = 9;
int m1LPin = 8;
int stepLPin = 7;
int dirLPin = 6;

//Transmitter Pin
int dataOutPin = 13;

//Servo Pin
int servoPin = 10;

ServoTimer2 grabberServo;
NewPing sonar(trigPin, echoPin, maxDistance);

bool moveForward = true;
bool lastState = true;

void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(9600);	  // Debugging only
  Serial.println("setup");
  
  pinMode(servoPin, OUTPUT);
  grabberServo.attach(servoPin);

  //Transmitter setup
  // Initialise the IO and ISR
  vw_set_ptt_inverted(true); // Required for DR3100
  vw_set_tx_pin(dataOutPin);
  vw_setup(2000);	 // Bits per sec
  
  StepperSetup();
  UltrasonicSetup();
}

void StepperSetup()
{
  pinMode(m0RPin, OUTPUT);
  pinMode(m1RPin, OUTPUT);
  pinMode(stepRPin, OUTPUT);
  pinMode(dirRPin, OUTPUT);
  
  digitalWrite(m0RPin, HIGH);
  digitalWrite(m1RPin, HIGH);
  digitalWrite(dirRPin, LOW);
  
  pinMode(m0LPin, OUTPUT);
  pinMode(m1LPin, OUTPUT);
  pinMode(stepLPin, OUTPUT);
  pinMode(dirLPin, OUTPUT);
  
  digitalWrite(m0LPin, HIGH);
  digitalWrite(m1LPin, HIGH);
  digitalWrite(dirLPin, HIGH);
}

void UltrasonicSetup()
{
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() 
{
  // put your main code here, to run repeatedly:
  SendDataToPequod("hello");
  //SendDataToPequod("d");
  //delay(5000);

  //MoveServo();
  
  if (moveForward)
  {
    MoveStepper(100);
    lastState = moveForward;
  }
  else
  {
    if (lastState != moveForward)
    {
      SendDataToPequod("d");
      lastState = moveForward;
    }
  }
  
  int distance = GetDistance(); //distance in cm
  if (distance < 40)
  {
    moveForward = false;
  }
  else
  {
    moveForward = true;
  }
}

void SendDataToPequod(char *msg)
{
  vw_send((uint8_t *)msg, strlen(msg));
  vw_wait_tx(); // Wait until the whole message is gone
}

void MoveServo()
{
  for (int x = 750; x <= 2250; x++)
  {
    Serial.println(x);
    grabberServo.write(x);
    delay(10);
  }
}

int GetDistance()
{
  unsigned int uS = sonar.ping();
  Serial.print("Ping: ");
  Serial.print(sonar.convert_cm(uS));
  Serial.println(" cm");
  //delay(1000);
  
  return sonar.convert_cm(uS);
}

void MoveStepper(int steps)
{
  for (int x = 0; x < steps; x++)
  {
    digitalWrite(stepRPin, HIGH);
    digitalWrite(stepLPin, HIGH);
    //delay(1);
    delayMicroseconds(500);
    digitalWrite(stepRPin, LOW);
    digitalWrite(stepLPin, LOW);
    //delay(1);
    delayMicroseconds(500);
  }
}
