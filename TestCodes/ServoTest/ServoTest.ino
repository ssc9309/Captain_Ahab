int servoPin = 7;


int lenMicroSecondsOfPeriod = 25 * 1000; // 25 milliseconds (ms)
int lenMicroSecondsOfPulse = 1 * 1000; // 1 ms is 0 degrees
//int first = 0.7 * 1000; //0.5ms is 0 degrees in HS-422 servo
int first = 600; //0.5ms is 0 degrees in HS-422 servo
//int end = 3.7 * 1000;
int end = 2400;
int increment = 0.01 * 1000;

void setup()
{
  Serial.begin(9600);
  pinMode(servoPin, OUTPUT);
}

//  int onPulse = 2400; //2400 is maximum speed, 600 maximum other way
int onPulse = (2400 + 600)/2; //the middle is stop
  //int onPulse = 1200;
int offPulse = 20000 - onPulse; //one cycle is 20000 microseconds long

void loop()
{
  //pulses are in microseconds
  digitalWrite(servoPin, HIGH);
  delayMicroseconds(onPulse);
  digitalWrite(servoPin, LOW);
  delayMicroseconds(offPulse);
  
  unsigned long average = 0;
  for (int x = 0; x < 100; x++)
  {
    average += analogRead(A3);
  }
  average /= 100;
  
  Serial.print(millis());
  Serial.print(": ");
  Serial.println(average);
}

void ThisWasOnlyApplicableToNonContinuousServ0()
{
      int current = 0;
    for(current = first; current <end; current+=increment){
              Serial.println(current);
         // Servos work by sending a 25 ms pulse.  
         // 0.7 ms at the start of the pulse will turn the servo to the 0 degree position
         // 2.2 ms at the start of the pulse will turn the servo to the 90 degree position 
         // 3.7 ms at the start of the pulse will turn the servo to the 180 degree position 
         // Turn voltage high to start the period and pulse
         digitalWrite(servoPin, HIGH);

         // Delay for the length of the pulse
         delayMicroseconds(current);

         // Turn the voltage low for the remainder of the pulse
         digitalWrite(servoPin, LOW);

         // Delay this loop for the remainder of the period so we don't
         // send the next signal too soon or too late
         delayMicroseconds(lenMicroSecondsOfPeriod - current);
        

    }
    for(current = end; current >first; current-=increment){
              Serial.println(current);
         // Servos work by sending a 20 ms pulse.
         // 0.7 ms at the start of the pulse will turn the servo to the 0 degree position
         // 2.2 ms at the start of the pulse will turn the servo to the 90 degree position
         // 3.7 ms at the start of the pulse will turn the servo to the 180 degree position
         // Turn voltage high to start the period and pulse
         digitalWrite(servoPin, HIGH);

         // Delay for the length of the pulse
         delayMicroseconds(current);

         // Turn the voltage low for the remainder of the pulse
         digitalWrite(servoPin, LOW);

         // Delay this loop for the remainder of the period so we don't
         // send the next signal too soon or too late
         delayMicroseconds(lenMicroSecondsOfPeriod - current);
    }
}
