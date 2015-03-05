byte counter;



void setup() {
  // put your setup code here, to run once:
  Serial.begin(4800);
  counter = 0;
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print(counter);
  counter++;
  delay(500);
}
