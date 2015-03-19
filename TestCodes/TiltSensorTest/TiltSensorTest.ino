int tiltPin = A3;
bool prevState = false;


void setup() 
{
  // put your setup code here, to run once:
  
  pinMode(tiltPin, INPUT);
  Serial.begin(9600);
}

void loop() 
{
  // put your main code here, to run repeatedly:
  if (prevState != digitalRead(tiltPin))
  {
    prevState = digitalRead(tiltPin);
    if (prevState)
    {
      Serial.println("Take a mooommeeennnttt");
    }
  }
}
