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
Arduino System Setup
-------------------------------------------*/
void setup(){

}

/*-------------------------------------------
Primary Execution
-------------------------------------------*/
void loop(){

}