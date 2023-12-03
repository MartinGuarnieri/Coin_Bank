/*!
 * @file readWeight.ino
 * @brief Get the weight of the object, press the RST button on the module, the 
 * @n program will automatically remove the skin
 * @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @License     The MIT License (MIT)
 * @author [fengli](li.feng@dfrobot.com)
 * @version  V1.0
 * @date  2020-12-26
 * @https://github.com/DFRobot/DFRobot_HX711_I2C
*/

#include <DFRobot_HX711_I2C.h>
#include <Wire.h>

/*!
 * @brief Constructor 
 * @param pWire I2c controller
 * @param addr  I2C address(0x64/0x65/0x660x67)
 */
//DFRobot_HX711_I2C MyScale(&Wire,/*addr=*/0x64);
DFRobot_HX711_I2C MyScale;

float value = 0;
float old_value;
float weight_diff;
float money = 0;
int result;

int digits[10] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x67};
int decimal = 0x80; //To turn on decimal point, bit wise and (&) with this
int error[3] = {0x79,0x31,0x31}; //Err to display invalid temperatures

int val;    //multiply value by 100 to use integer math
int first; //first 7-segment digit
int second; //second 7-segment digit
int third; //third 7-segment digit
int fourth; //fourth 7-segment digit


uint8_t displayAddr = 0x70;
uint8_t buffer[17]; //char buffer to write to display

void setup() {

  Serial.begin(9600);
  while (!MyScale.begin()) {
    Serial.println("The initialization of the chip is failed, please confirm whether the chip connection is correct");
    delay(1000);
  }
  //Manually set the calibration values
  MyScale.setCalibration(2236.f);
  //remove the peel
  MyScale.peel();
  
  delay(2000);

  Wire.begin();
  Wire.setClock(100000);
  Wire.beginTransmission(displayAddr);

  /* Turn on oscillator */
	buffer[0]=(0x2<<4)|(0x1);
  Wire.beginTransmission(displayAddr);
	result=Wire.write(buffer, 1);
  Wire.endTransmission(0x70);

	/* Turn on Display, No Blink */
	buffer[0]=(0x8<<4)|0x1;
  Wire.beginTransmission(displayAddr);
	result=Wire.write(buffer, 1);
  Wire.endTransmission(0x70);
	Serial.println(result);

	/* Set Brightness */
	buffer[0]=(0xE<<4)|0xA;
  Wire.beginTransmission(displayAddr);
	result=Wire.write(buffer, 1);
  Wire.endTransmission(0x70);

}

void loop() {
  old_value = value;
  delay(500);
  value = MyScale.readWeight();
  delay(500);
  weight_diff = value - old_value;
  if (weight_diff > 0.5)
  {
    delay(500);
    value = MyScale.readWeight();
    weight_diff = value - old_value;
  }
  if (1.4 <= weight_diff && weight_diff <= 1.82) //+dime
    money += 0.1;
  else if (1.82 < weight_diff && weight_diff < 2.8) //+penny
    money += 0.01;
  else if (2.8 <= weight_diff && weight_diff <= 3.9) //+nickel
    money += 0.05;
  else if (3.9 <= weight_diff && weight_diff <= 5) //+quarter
    money += 0.25;
  
  val = money * 100; //to do integer math
  buffer[0]=0x00; // offset pointer
  if (0 <= money && money <= 9.99){ //0 to 9.9 
    fourth = val % 10;
    val /= 10;
    third = val % 10; //get the decimal place
    val/=10;          
    second = val % 10; //get the ones place
    
    buffer[1] = 0x00; // clear first digit to prevent leading zeros
    buffer[3] = digits[second] | decimal;//update  ones digit with decimal point
    buffer[7] = digits[third]; //update decimal digit
    buffer[9] = digits[fourth];
  }
  else if (9.9 < money && money <= 99.99){ //9.9 to 99.9
    fourth = val % 10;
    val /= 10;
    third = val % 10; //get the decimal place
    val/=10;
    second = val % 10; //get the ones place
    val /= 10;
    first = val % 10; //get the tens place
    
    buffer[1] = digits[first]; // update the tens place
    buffer[3] = digits[second] | decimal;//update the ones place with decimal point
    buffer[7] = digits[third]; //update the decimal place 
    buffer[9] = digits[fourth];
  }
  else{ //Otherwise an invalid value
    buffer[1] = error[0]; // E
    buffer[3] = error[1]; // r
    buffer[7] = error[2]; // r
    buffer[9] = 0;
  }
  Wire.beginTransmission(0x70);
  Wire.write(buffer,17); //write buffer to device
  Wire.endTransmission(0x70);
  

}