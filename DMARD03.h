/*
* Example I2C header code for DMARD01 g-sensor I2C coding
* Date: 2009/09/14
* Version: V1.0
* Company: DMT
*
* Porting to Arduino by coopermaa77@gmail.com 
* Date: 2011/04/08
*/

#ifndef __DMARD03_H
#define __DMARD03_H

#include <WProgram.h> // for Arduino functions, such as pinMode(), digitalWrite()

#define SCL _clockPin 
#define SDA _dataPin

//Need to define the following Macros
#define SET_I2C_CLK_OUTPUT       { pinMode(SCL, OUTPUT); } 
#define SET_I2C_DATA_OUTPUT      { pinMode(SDA, OUTPUT); } 
#define SET_I2C_DATA_INPUT       { pinMode(SDA, INPUT); } 
#define SET_I2C_CLK_HIGH         { digitalWrite(SCL, HIGH); } 
#define SET_I2C_CLK_LOW          { digitalWrite(SCL, LOW); } 
#define SET_I2C_DATA_HIGH        { digitalWrite(SDA, HIGH); } 
#define SET_I2C_DATA_LOW         { digitalWrite(SDA, LOW); }
#define GET_I2C_DATA_BIT          digitalRead(SDA) // input SDA

// Slave address for SDO(pin17) connected to GND
#define DMARD01_I2C_SLAVE_WRITE_ADDR 0x38
#define DMARD01_I2C_SLAVE_READ_ADDR  0x39

// Slave address for SDO(pin17) connected to VCC
//#define DMARD01_I2C_SLAVE_WRITE_ADDR 0x3a
//#define DMARD01_I2C_SLAVE_READ_ADDR  0x3b

// Start register address for T, X, Y, Z
#define DMARD01_T_REG_START_ADDRESS 0x00
#define DMARD01_X_REG_START_ADDRESS 0x02
#define DMARD01_Y_REG_START_ADDRESS 0x04
#define DMARD01_Z_REG_START_ADDRESS 0x06

class DMARD03_results
{
  public:
    int X, Y, Z, T;
};

class DMARD03
{
  public:
    DMARD03(int dataPin, int clockPin);
    void readXYZ(DMARD03_results &results);
  private:
    int _dataPin;
    int _clockPin;

    void acc_sensor_init();    
    char acc_sensor_write_bytes(unsigned char reg_addr, unsigned char* reg_data, unsigned char wr_len);
    char acc_sensor_read_bytes(unsigned char reg_addr, unsigned char* reg_data, unsigned char r_len);    
};    

#endif
