#include <VirtualWire.h>


byte counter;
int dataOutputPin = 2;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  counter = 0;
  
  
  // Initialise the IO and ISR
  vw_set_ptt_inverted(true); // Required for DR3100
  vw_setup(2000);	 // Bits per sec
  vw_set_tx_pin(dataOutputPin); //Pin 3 is connected to "Digital Output" of receiver
  
}

void loop() {
  // put your main code here, to run repeatedly:
  /*
  Serial.print(counter);
  counter++;
  delay(500);
  */
  
  VirtualWireTest();
}

void VirtualWireTest()
{
  char *msg;
  
  msg = "1";
  
  vw_send((uint8_t *)msg, strlen(msg));
  vw_wait_tx();
  Serial.println(msg);
  
  delay(1000);
  
  msg = "a";
  
  vw_send((uint8_t *)msg, strlen(msg));
  vw_wait_tx();
  
  Serial.println(msg);
  delay(1000);
}
