// ATMEL ATTINY45 / ARDUINO
//
//                           +-\/-+
//  Ain0       (D  5)  PB5  1|    |8   VCC
//  Ain3       (D  3)  PB3  2|    |7   PB2  (D  2)  INT0  Ain1 SCL
//  Ain2       (D  4)  PB4  3|    |6   PB1  (D  1)        pwm1
//                     GND  4|    |5   PB0  (D  0)  SDA   pwm0
//                           +----+

#include <TinyWireS.h>
#define MY_I2C_SLAVE_ADDR 3

void setup(){
  TinyWireS.begin(MY_I2C_SLAVE_ADDR);
}

void loop(){

}


