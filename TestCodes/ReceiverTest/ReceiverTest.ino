//TX out of Arduino is Digital Pin 1
//RX into Arduino is Digital Pin 0

//following link for lcd screen
//http://www.instructables.com/id/Connecting-an-LCD-to-the-Arduino/

#include <LiquidCrystal.h>


int incomingByte = 0;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() 
{
  // put your setup code here, to run once:
  lcd.begin(16, 2);
  
  //pretty sure our model is 4800. If this doesn't work, use 2400
  Serial.begin(4800);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0)
  {
    incomingByte = Serial.read();
    //Serial.print(millis());
    //Serial.print(" : ");
    //Serial.println(incomingByte, DEC);
    
    lcd.setCursor(0, 0);
    lcd.print(incomingByte);
  }
  
  incomingByte = -1;
  
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print(millis() / 1000);
}
