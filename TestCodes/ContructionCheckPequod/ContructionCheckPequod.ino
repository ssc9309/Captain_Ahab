#include <VirtualWire.h>
#include <LiquidCrystal.h>
#include <ServoTimer2.h>


LiquidCrystal lcd(13, 11, 5, 4, 3, 2);

int dataReceiverPin = 12;
int servoPin = 7;

int timer = 0;

ServoTimer2 reelerServo;


void setup() 
{
  // put your setup code here, to run once:
  lcd.begin(16, 2);

  Serial.begin(9600);	// Debugging only
  Serial.println("setup");


  //pinMode(servoPin, HIGH);
  reelerServo.attach(servoPin);
  
  //Receiver Setup
  // Initialise the IO and ISR
  vw_set_ptt_inverted(true); // Required for DR3100
  vw_set_rx_pin(dataReceiverPin);  
  vw_setup(2000);	 // Bits per sec
  vw_rx_start();       // Start the receiver PLL running
}

void loop() 
{
  // put your main code here, to run repeatedly:
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;

  String dataReceived = "";

  if (vw_get_message(buf, &buflen)) // Non-blocking
  {
   
    // Message with a good checksum received, dump it.
    Serial.print("Data Received: ");
	
    for (int i = 0; i < buflen; i++)
    {
      //Serial.print(buf[i], HEX);
      Serial.print(buf[i]);
      Serial.print(" ");
      
      //buf[i] will be form of dec, which is from hex, which is from char.
      //http://web.cs.mun.ca/~michael/c/ascii-table.html
      dataReceived = String(dataReceived + buf[i] + " ");
    }
    
    Serial.println("");
    
    lcd.setCursor(0, 0);
    lcd.print(dataReceived);
    
    //100 is 0x64, which is lower case d
    if (dataReceived.substring(0, 3) == "100")
    {
      //RunServo();
      SetServoSpeed();
    }
  }
    
  if (timer != millis()/1000)
  {
    Serial.println(timer);
    lcd.setCursor(0, 1);
    lcd.print(timer);
    timer = millis()/1000;
  }
}

void RunServo()
{
  //https://www.servocity.com/html/hs-645mg_ultra_torque.html#.VPlIB_nF9-c
  // 600 us is 0 degree
  //2400 us is 180 degree
  
  
  int lenMicroSecondsOfPeriod = 25 * 1000; // 25 milliseconds (ms)
  int lenMicroSecondsOfPulse = 1 * 1000; // 1 ms is 0 degrees
//int first = 0.7 * 1000; //0.5ms is 0 degrees in HS-422 servo
  int firstDegree = 600; //0.5ms is 0 degrees in HS-422 servo
//int end = 3.7 * 1000;
  int lastDegree = 2400;
  int increment = 0.01 * 1000;
  
  int current = 0;
  for(current = firstDegree; current <lastDegree; current+=increment)
  {
    //Serial.println(current);
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
  for(current = lastDegree; current >firstDegree; current-=increment)
  {
    //Serial.println(current);
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

void SetServoSpeed()
{
  
}
