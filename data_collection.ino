#include <Wire.h>
extern "C" {
  #include "utility/twi.h"
}
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_L3GD20_U.h>

/*-------------------------------------------
Definition of fixed values
-------------------------------------------*/
//MUX I2C Address
#define MUX_ADDRESS 0x70

/*-------------------------------------------
Gyroscope Object
-------------------------------------------*/
Adafruit_L3GD20_Unified gyro = Adafruit_L3GD20_Unified();
double gyro_forces[3] = {0};

/*-------------------------------------------
Accelerometer Objects and Variables
-------------------------------------------*/
Adafruit_MMA8451 accelerometers[8];
uint8_t foundAccels;
double accel_forces[8][3] = {0};

/*-------------------------------------------
Arduino System Setup
-------------------------------------------*/
void setup(){

  if(!gyro.begin()){
    Serial.println("You screwed up. The gyroscope is not available.");
    while(true){}
  }

  //Initialize Accelerometers
  foundAccels = initAccel(0) | initAccel(1) | initAccel(2) | initAccel(3) | initAccel(4) | initAccel(5) | initAccel(6) | initAccel(7);

  //If no accelerometers are initialized stop.
  if(foundAccels == 0){
    Serial.println("You screwed up. There are no accelerometers available.");
    while(true){}
  }
}

/*-------------------------------------------
Primary Execution
-------------------------------------------*/
void loop(){
  sensors_event_t se;

  //Gyroscope
  gyro.getEvent(&se);
  gyro_forces[0] = se.gyro.x;
  gyro_forces[1] = se.gyro.y;
  gyro_forces[2] = se.gyro.z;

  //Accelerometers
  for(int i = 0; i < 8; i += 1){
    if((foundAccels & (0x1 << i)) == (0x1 << i)){
      accelerometers[i].getEvent(&se);

      accel_forces[i][0] = se.acceleration.x;
      accel_forces[i][1] = se.acceleration.y;
      accel_forces[i][2] = se.acceleration.z;
    }
  }

  //Print data to serial console
  Serial.print(gyro_forces[0]); Serial.print(","); Serial.print(gyro_forces[1]); Serial.print(","); Serial.print(gyro_forces[2]); Serial.print(",");

  for(int i = 0; i < 8; i += 1){
    for(int j = 0; j < 3; j += 1){
      Serial.print(accel_forces[i][j]);
      if(j < 2){
        Serial.print(",");
      }
    }
    Serial.print("\n");
  }

  delay(500);
}

/*-------------------------------------------
Select Port of MUX to use
-------------------------------------------*/
void muxSelect(uint8_t i) {
  if(i > 7){ return; }

  //Send selected port to MUX
  Wire.beginTransmission(MUX_ADDRESS);
  Wire.write(1 << i);
  Wire.endTransmission();
}

/*-------------------------------------------
Accelerometer Initialization Function
-------------------------------------------*/
bool initAccel(uint8_t i){
  if(i > 7){ return false; }

  muxSelect(i);
  accelerometers[i] = Adafruit_MMA8451();
  if(!accelerometers[i].begin(0x16)){
    Serial.print("No accelerometer dectected in slot ");
    Serial.print(i);
    Serial.println(".");
    return false;
  }

  return true;
}
