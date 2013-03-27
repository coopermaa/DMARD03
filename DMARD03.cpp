/*
* Example I2C function code for DMARD01 g-sensor I2C coding
* Date: 2009/09/14
* Version: V1.1
* Company: DMT
*/

#include "WProgram.h"
#include "DMARD03.h"

// Need to define the following delay MACRO
#define SHORT_DELAY { delayMicroseconds(3); }  // short delay, at least 2us
#define LONG_DELAY  { delayMicroseconds(3); } // long delay, at least 2us

DMARD03::DMARD03(int dataPin, int clockPin)
{
  _dataPin = dataPin;
  _clockPin = clockPin;
    
  acc_sensor_init();
}

void DMARD03::readXYZ(DMARD03_results &results)
{
  unsigned char data[8], data_len = 8;
  unsigned char adc_bit = 11;
  
  acc_sensor_read_bytes(DMARD01_T_REG_START_ADDRESS, &data[0], data_len);
    
  // combine two bytes to get the 11-bit acceleration representation, see AN001  
  results.T = (data[0]<<3)|(data[1]&7);                         
  if (results.T & (1<<(adc_bit-1))) results.T -= (1<<adc_bit);  // 2's complement to negative 
  
  results.X = (data[2]<<3)|(data[3]&7);  
  if (results.X & (1<<(adc_bit-1))) results.X -= (1<<adc_bit);  // 2's complement to negative 

  results.Y = (data[4]<<3)|(data[5]&7);  
  if (results.Y & (1<<(adc_bit-1))) results.Y -= (1<<adc_bit);  // 2's complement to negative 

  results.Z = (data[6]<<3)|(data[7]&7);  
  if (results.Z & (1<<(adc_bit-1))) results.Z -= (1<<adc_bit);  // 2's complement to negative 
  
}


void DMARD03::acc_sensor_init(void)
{
  unsigned char reg_add=8, data;
  // set 08h to 7: sampling freq of 684Hz, no filtering
  // data = 7;

  // set 08h to 6: sampling freq of 684Hz, filter bandwidth 153Hz
  // data = 6;
  
  // set 08h to 5: sampling freq of 684Hz, filter bandwidth 76Hz
  // data = 5;

  // set 08h to 4: sampling freq of 684Hz, filter bandwidth 38Hz
  // data = 4;

  // set 08h to 3: sampling freq of 342Hz, no filtering
  // data = 3;
  
  // set 08h to 2: sampling freq of 342Hz, filter bandwidth 76Hz
  // data = 2;
  
  // set 08h to 1: sampling freq of 342Hz, filter bandwidth 38Hz
  // data = 1;
  
  // set 08h to 0: sampling freq of 342Hz, filter bandwidth 19Hz
  // data = 0;
  
  acc_sensor_write_bytes(reg_add, &data, 1);  
}

char DMARD03::acc_sensor_write_bytes(unsigned char reg_addr, unsigned char* reg_data, 
                             unsigned char wr_len)
{
   int i, j, k;
  
   /*----- START -----*/
   //ST: a falling edge in SDA while SCK is high
   SET_I2C_DATA_OUTPUT;
   SET_I2C_CLK_OUTPUT;
   SET_I2C_DATA_HIGH;
   SET_I2C_CLK_HIGH;
   SHORT_DELAY;
   SET_I2C_DATA_LOW;
   SHORT_DELAY;
   SET_I2C_CLK_LOW;

   /*----- SEND -----*/
   //----- Write Command --------
   // Send slave write address
   LONG_DELAY;

   for (i = 7; i >= 0; i--){
      if (DMARD01_I2C_SLAVE_WRITE_ADDR &(1 << i)){
         SET_I2C_DATA_HIGH;
      }
      else{
         SET_I2C_DATA_LOW;
      }
      SHORT_DELAY;
      SET_I2C_CLK_HIGH;      
      LONG_DELAY;
      SET_I2C_CLK_LOW;
   }

   /* don't care bit, 9th bit */
   //A: acknowledge from slave
   SET_I2C_DATA_LOW;
   SET_I2C_DATA_INPUT;
   SHORT_DELAY;
   SET_I2C_CLK_HIGH;
   LONG_DELAY;
   SET_I2C_CLK_LOW;
   SET_I2C_DATA_OUTPUT;

   //----- Read Adress --------
   //Reg address
   for (i = 7; i >= 0; i--){
      if ((reg_addr) &(1 << i)){
         SET_I2C_DATA_HIGH;
      }
      else{
         SET_I2C_DATA_LOW;
      }
      SHORT_DELAY; //Extra
      SET_I2C_CLK_HIGH;
      LONG_DELAY;
      SET_I2C_CLK_LOW;
   }

   /* don't care bit, 9th bit */
   //A: acknowledge from slave
   SET_I2C_DATA_LOW;
   SET_I2C_DATA_INPUT;
   SHORT_DELAY;
   SET_I2C_CLK_HIGH;
   LONG_DELAY;
   SET_I2C_CLK_LOW;
   SET_I2C_DATA_OUTPUT;

   
	//----- Send data out now --------
	// Read  data
	 LONG_DELAY;
   
   for (k = 0; k < wr_len; k++){

	   for (i = 7; i >= 0; i--){
	      if ( reg_data[k] &(1 << i)){
	         SET_I2C_DATA_HIGH;
	      }
	      else{
	         SET_I2C_DATA_LOW;
	      }
	      SHORT_DELAY;
	      SET_I2C_CLK_HIGH;
	      LONG_DELAY;
	      SET_I2C_CLK_LOW;
	   }

      if (k != wr_len - 1){
         // don't care bit, 9th bit
         //A: acknowledge from slave
         SET_I2C_DATA_OUTPUT;
         SET_I2C_DATA_LOW;
         LONG_DELAY;
         SET_I2C_CLK_HIGH;
         LONG_DELAY;
         SET_I2C_CLK_LOW;
      }
      else
      {
         /*Last byte use NACK, skip ACK*/
         //NA: a falling edge on SCL when SDA is high
         SET_I2C_DATA_OUTPUT;
         SET_I2C_DATA_HIGH;
         SHORT_DELAY;
         SET_I2C_CLK_HIGH;
         SHORT_DELAY;
         SET_I2C_CLK_LOW;
      }
   }
 
   /*----- STOP -----*/
   //SP: a rising edge in SDA while SCL is high
   SET_I2C_DATA_OUTPUT;
   SET_I2C_CLK_LOW;
   SET_I2C_DATA_LOW;
   SHORT_DELAY;
   SET_I2C_CLK_HIGH;
   SHORT_DELAY;
   SET_I2C_DATA_HIGH;

   return 1;
}

char DMARD03::acc_sensor_read_bytes(unsigned char reg_addr, unsigned char* reg_data,
														unsigned char r_len)
{
   int i, j, k;

   //reset data to 0
   for (k = 0; k < r_len; k++){
      *(reg_data + k) = 0;
   }
   
   /*----- START -----*/
   //ST: a falling edge in SDA while SCK is high
   SET_I2C_DATA_OUTPUT;
   SET_I2C_CLK_OUTPUT;
   SET_I2C_DATA_HIGH;
   SET_I2C_CLK_HIGH;
   SHORT_DELAY;
   SET_I2C_DATA_LOW;
   SHORT_DELAY;
   SET_I2C_CLK_LOW;

   /*----- SEND -----*/
   //----- Write Command --------
   // Send slave write address
   LONG_DELAY;

   for (i = 7; i >= 0; i--){
      if (DMARD01_I2C_SLAVE_WRITE_ADDR &(1 << i)){
         SET_I2C_DATA_HIGH;
      }
      else{
         SET_I2C_DATA_LOW;
      }
      SHORT_DELAY;
      SET_I2C_CLK_HIGH;
      LONG_DELAY;
      SET_I2C_CLK_LOW;
   }

   /* don't care bit, 9th bit */
   //A: acknowledge from slave
   SET_I2C_DATA_LOW;
   SET_I2C_DATA_INPUT;
   SHORT_DELAY;
   SET_I2C_CLK_HIGH;
   LONG_DELAY;
   SET_I2C_CLK_LOW;
   SET_I2C_DATA_OUTPUT;

   //----- Read Adress --------
   //Reg address
   for (i = 7; i >= 0; i--){
      if ((reg_addr) &(1 << i)){
         SET_I2C_DATA_HIGH;
      }
      else{
         SET_I2C_DATA_LOW;
      }
      SHORT_DELAY; //Extra
      SET_I2C_CLK_HIGH;
      LONG_DELAY;
      SET_I2C_CLK_LOW;
   }

   /* don't care bit, 9th bit */
   //A: acknowledge from slave
   SET_I2C_DATA_LOW;
   SET_I2C_DATA_INPUT;
   SHORT_DELAY;
   SET_I2C_CLK_HIGH;
   LONG_DELAY;
   SET_I2C_CLK_LOW;
   SET_I2C_DATA_OUTPUT;

   /*----- RESTART -----*/
   //SR: repeated start condition, a falling edge on SDA while SCL is high
   SET_I2C_DATA_HIGH;
   SET_I2C_CLK_HIGH;
   SHORT_DELAY;
   SET_I2C_DATA_LOW;
   SHORT_DELAY;
   SET_I2C_CLK_LOW;


   //----- Read Command --------
   //Slave read address
   SHORT_DELAY; //Extra
   for (i = 7; i >= 0; i--){
      if ((DMARD01_I2C_SLAVE_READ_ADDR) &(1 << i)){
         SET_I2C_DATA_HIGH;
      }
      else{
         SET_I2C_DATA_LOW;
      }
      SHORT_DELAY; //Extra
      SET_I2C_CLK_HIGH;
      LONG_DELAY;
      SET_I2C_CLK_LOW;
   }

   /* don't care bit, 9th bit */
   //A: acknowledge from slave
   SET_I2C_DATA_LOW;
   SET_I2C_DATA_INPUT;
   SHORT_DELAY;
   SET_I2C_CLK_HIGH;
   LONG_DELAY;
   SET_I2C_CLK_LOW;
   SET_I2C_DATA_OUTPUT;

	//----- Read data out now --------
	// Read  data
   for (k = 0; k < r_len; k++){

      SET_I2C_DATA_INPUT;

      for (i = 7; i >= 0; i--){
         SET_I2C_CLK_HIGH;
         SHORT_DELAY;
         if (GET_I2C_DATA_BIT)
            *(reg_data + k) |= (1 << i);
         SET_I2C_CLK_LOW;
         LONG_DELAY;
      }

      if (k != r_len - 1){
         // don't care bit, 9th bit
         //A: acknowledge from slave
         SET_I2C_DATA_OUTPUT;
         SET_I2C_DATA_LOW;
         LONG_DELAY;
         SET_I2C_CLK_HIGH;
         LONG_DELAY;
         SET_I2C_CLK_LOW;
      }
      else
      {
         /*Last byte use NACK, skip ACK*/
         //NA: a falling edge on SCL when SDA is high
         SET_I2C_DATA_OUTPUT;
         SET_I2C_DATA_HIGH;
         SHORT_DELAY;
         SET_I2C_CLK_HIGH;
         SHORT_DELAY;
         SET_I2C_CLK_LOW;
      }
   }
 
   /*----- STOP -----*/
   //SP: a rising edge in SDA while SCL is high
   SET_I2C_DATA_OUTPUT;
   SET_I2C_CLK_LOW;
   SET_I2C_DATA_LOW;
   SHORT_DELAY;
   SET_I2C_CLK_HIGH;
   SHORT_DELAY;
   SET_I2C_DATA_HIGH;

   return 1;

}
