//TX out of Arduino is Digital Pin 1
//RX into Arduino is Digital Pin 0

//following link for lcd screen
//http://www.instructables.com/id/Connecting-an-LCD-to-the-Arduino/

#include <LiquidCrystal.h>
#include <VirtualWire.h>


int incomingByte = 0;
int receiverPin = 8;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() 
{
  // put your setup code here, to run once:
  lcd.begin(16, 2);
  
  //pretty sure our model is 4800. If this doesn't work, use 2400
  Serial.begin(9600);
  
  
  //Initialize the IO and ISR
  vw_set_ptt_inverted(true); // Required for DR3100
  vw_setup(2000);	    // Bits per sec
  vw_set_rx_pin(receiverPin);         //Pin 2 is connected to "Digital Output" of receiver
  vw_rx_start();           // Start the receiver PLL running
  
  
}

void loop() {
  // put your main code here, to run repeatedly:
  /*
  if (Serial.available() > 0)
  {
    incomingByte = Serial.read();
    //Serial.print(millis());
    //Serial.print(" : ");
    //Serial.println(incomingByte, DEC);
    
    lcd.setCursor(0, 0);
    lcd.print(incomingByte);
  }
  */
  
  VirtualWareTest();
  
  
  
  
  incomingByte = -1;
  
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print(millis() / 1000);
}

void VirtualWareTest()
{
  
  //Set buffer array based on max message length
  uint8_t buf[VW_MAX_MESSAGE_LEN];

  //Set variable to indicate buffer length
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  
  if (vw_get_message(buf, &buflen)) // Non-blocking
  {
    // Flash status LED to show received data
    //digitalWrite(13, true); 

    // Message with a good checksum received, dump it.
    Serial.print("Got: ");

    for (int i = 0; i < buflen; i++)
    {
      
      // Print message received in buffer through this loop
      Serial.print(buf[i]);
      //add space to distinguish characters from each other if showing ASCII decimal #
      //Serial.print(" "); 
      
      /*
      //if character received matches, turn on associated LED
      if(buf[i] == '1'){
        digitalWrite(8, HIGH);
      }
      if(buf[i] == '2'){
        digitalWrite(9, HIGH);
      }
      if(buf[i] == '3'){
        digitalWrite(10, HIGH);
      }
      if(buf[i] == '4'){
        digitalWrite(11, HIGH);
      }
      */
    }
    //Print next character on new line
    Serial.println("");

    //Turn off status LED
    //digitalWrite(13, false);
  }
}
