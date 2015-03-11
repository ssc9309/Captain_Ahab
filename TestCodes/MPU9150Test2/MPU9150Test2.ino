#include "Wire.h"
#include "SPI.h"

// Include the following libraries for the MPU 6050 IC, I2Cdev and AK8975 (Magnetometer)

#include "I2Cdev.h"
#include "MPU6050.h"
#include "AK8975.h"

// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for InvenSense evaluation board)
// AD0 high = 0x69
AK8975 mag(0x0C);
MPU6050 accelgyro;
unsigned long time;

int16_t ax, ay, az;
int16_t gx, gy, gz;
int16_t mx, my, mz;

float heading;

#define LED_PIN 13
bool blinkState = false;

void setup() {
    // join I2C bus (I2Cdev library doesn't do this automatically)
    Wire.begin();

    // Set baud rate to 115200 (figure out ways to double it)
    Serial.begin(115200);

    // initialize device
    Serial.println("Initializing I2C devices...");
   
    accelgyro.initialize();
   accelgyro.setI2CBypassEnabled(true);
   mag.initialize();

    // verify connection
    Serial.println("Testing device connections...");
    Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
   Serial.println(mag.testConnection() ? "AK8975 connection successful" : "AK8975 connection failed");

    accelgyro.setFullScaleAccelRange(0);

    // configure Arduino LED for
    pinMode(LED_PIN, OUTPUT);
}


void loop() {

    time = millis();
    //prints time since program started


    // read raw accel/gyro measurements from device
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
   mag.getHeading(&mx, &my, &mz);

    // these methods (and a few others) are also available
    // accelgyro.getAcceleration(&ax, &ay, &az);
    // accelgyro.getRotation(&gx, &gy, &gz);
   // mag.getHeading(&mx, &my, &mz);

    // display tab-separated accel/gyro x/y/z values
    // Serial.print("a/g/m/Time:\t");
    Serial.print(ax); Serial.print("\t");
    Serial.print(ay); Serial.print("\t");
    Serial.print(az); Serial.print("\t");
    Serial.print(gx); Serial.print("\t");
    Serial.print(gy); Serial.print("\t");
    Serial.print(gz); Serial.print("\t");
    Serial.print(mx); Serial.print("\t");
    Serial.print(my); Serial.print("\t");
    Serial.print(mz); Serial.print("\t");
    Serial.println(time);
   
    // blink LED to indicate activity
    blinkState = !blinkState;
    digitalWrite(LED_PIN, blinkState);
   //delay(10); // run at ~100 Hz
}
