#include <Wire.h>
extern "C" { 
  #include "utility/twi.h"
}
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_L3D20_U.h>

/*-------------------------------------------
Definition of fixed values
-------------------------------------------*/
//MUX I2C Address 
#define MUX_ADDRESS 0x70

/*-------------------------------------------
Accelerometer Objects and Variables
-------------------------------------------*/
Adafruit_MMA8451 accelerometers[8];
uint8_t foundAccels;
double forces[8][3] = {0};

/*-------------------------------------------
Arduino System Setup
-------------------------------------------*/
void setup(){
  
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

  for(int i = 0; i < 8; i += 1){
    sensors_event_t se;
  
    if((foundAccels & (0x1 << i)) == (0x1 << i)){
      accelerometers[i].getEvent(&se);
      
      forces[i][0] = se.acceleration.x;
      forces[i][1] = se.acceleration.y;
      forces[i][2] = se.acceleration.z;
    }
  }
  
  
  for(int i = 0; i < 8; i += 1){
    for(int j = 0; j < 3; j += 1){
      Serial.print(forces[i][j]);
      if(j < 2){
        Serial.print(",");
      }
    }
    Serial.print("\n");
  }

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