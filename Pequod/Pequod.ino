//TX out of Arduino is Digital Pin 1
//RX into Arduino is Digital Pin 0

int incomingByte = 0;


void setup() 
{
  // put your setup code here, to run once:

  //pretty sure our model is 4800. If this doesn't work, use 2400
  Serial.begin(4800);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0)
  {
    incomingByte = Serial.read();
    Serial.println(incomingByte, DEC);
  }
  
  incomingByte = -1;
}
