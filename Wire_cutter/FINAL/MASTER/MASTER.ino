//MASTER

/*

// ATMEL ATTINY84 / ARDUINO
//
//                           +-\/-+
//                     VCC  1|    |14  GND
//             (D  0)  PB0  2|    |13  PA0  (D 10)        AREF
//             (D  1)  PB1  3|    |12  PA1  (D  9) 
//             (D 11)  PB3  4|    |11  PA2  (D  8) 
//  PWM  INT0  (D  2)  PB2  5|    |10  PA3  (D  7) 
//  PWM        (D  3)  PA7  6|    |9   PA4  (D  6) 
//  PWM        (D  4)  PA6  7|    |8   PA5  (D  5)        PWM
//                           +----+
 3 PINS STEPPER
 7 PINS KEYPAD
 
 8/10 USED
 
 */


#include <TinyWireM.h>
#include <Keypad.h>
#define LCD_I2C_SLAVE_ADDR 2
#define MOTOR_I2C_SLAVE_ADDR 3

#define STEPPER_MOTOR_STEPS_PER_REV 200
#define DIAMETER_OF_WIRE_FEED 1
#define MICROSTEPPING_AMOUNT 4
#define RPS_OF_MOTOR 2
#define STEPPER_MOTOR_PULSE_PIN 1
#define STEPPER_MOTOR_ENABLE_PIN 2
#define STEPPER_MOTOR_REVERSE_PIN 3


const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {
    '1','2','3'                                                                                          }
  ,
  {
    '4','5','6'                                                                                          }
  ,
  {
    '7','8','9'                                                                                          }
  ,
  {
    '#','0','*'                                                                                          }
};
byte rowPins[ROWS] = {
  7, 8, 9, 10}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {
  11, 12, 12}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

float CIRC_PER_STEP = ((2*PI*(DIAMETER_OF_WIRE_FEED/2))/(STEPPER_MOTOR_STEPS_PER_REV*MICROSTEPPING_AMOUNT));

void setup(){
  TinyWireM.begin();
  pinMode(STEPPER_MOTOR_PULSE_PIN, OUTPUT);
  pinMode(STEPPER_MOTOR_ENABLE_PIN, OUTPUT);
  pinMode(STEPPER_MOTOR_REVERSE_PIN, OUTPUT);

}

void loop(){
  TinyWireM.beginTransmission(LCD_I2C_SLAVE_ADDR);
  TinyWireM.send(1);
  TinyWireM.endTransmission();
  int LENGTH_OF_CUTS = KEYPAD_VALUE();
  TinyWireM.beginTransmission(LCD_I2C_SLAVE_ADDR);
  TinyWireM.send(2);
  TinyWireM.endTransmission();
  int NUMBER_OF_CUTS = KEYPAD_VALUE();
  TinyWireM.beginTransmission(LCD_I2C_SLAVE_ADDR);
  TinyWireM.send(3);
  //
  //
  TinyWireM.endTransmission();
  TinyWireM.beginTransmission(LCD_I2C_SLAVE_ADDR);
  TinyWireM.send(20);
  TinyWireM.endTransmission();
  FEEDER_UPDATE(LENGTH_OF_CUTS,NUMBER_OF_CUTS,0,0);
  ENABLE_STEPPER(); 
  START_FEED(3);
  START_PROCESSING(LENGTH_OF_CUTS,NUMBER_OF_CUTS);
  START_FEED(-3);
  DISABLE_STEPPER();
}

int KEYPAD_VALUE(){
  int x = 0;
  LCD_setCursor(x,1);
  LCD_cursor();
  LCD_blink();
  int value;

  char key[] = {
    'x','x','x','x','x'                            };
  while (1){
    key[x] = keypad.getKey();
    if (key[x] != NO_KEY){
      if (key[x] == '*' and x != 0){
        x--;
      } 
      else if (key[x] == '#' and x != 0){
        switch (x){
        case 1:
          value = (key[0]-48);
          break;
        case 2:
          value = (((key[0]-48)*10)+(key[1]-48));
          break;
        case 3:
          value = (((key[0]-48)*100)+((key[1]-48)*10)+(key[2]-48));
          break;
        case 4:
          value = (((key[0]-48)*1000)+((key[1]-48)*100)+((key[2]-48)*10)+(key[3]-48));
          break;
        }
        return value;
      }
      else
        LCD_setCursor(x,1);
      LCD_print(key[x]);
      x++;
    }
  }
}


int LCD_setCursor(int Col, int Row){
  TinyWireM.beginTransmission(LCD_I2C_SLAVE_ADDR);
  TinyWireM.send(22);
  TinyWireM.send(Col);
  TinyWireM.send(Row);
  TinyWireM.endTransmission();
}

void LCD_cursor(){
  TinyWireM.beginTransmission(LCD_I2C_SLAVE_ADDR);
  TinyWireM.send(23);
  TinyWireM.endTransmission();
}

void LCD_blink(){
  TinyWireM.beginTransmission(LCD_I2C_SLAVE_ADDR);
  TinyWireM.send(25);
  TinyWireM.endTransmission();
}

char LCD_print(char value){
  TinyWireM.beginTransmission(LCD_I2C_SLAVE_ADDR);
  TinyWireM.send(27);
  TinyWireM.send(value);
  TinyWireM.endTransmission();
}

void FEEDER_UPDATE(int LENGTH_OF_CUTS,int NUMBER_OF_CUTS,int LENGTH_FED,int CUTS_MADE){
  TinyWireM.beginTransmission(LCD_I2C_SLAVE_ADDR);
  TinyWireM.send(4);
  TinyWireM.send(LENGTH_OF_CUTS);
  TinyWireM.send(NUMBER_OF_CUTS);
  TinyWireM.send(LENGTH_FED);
  TinyWireM.send(CUTS_MADE);
  TinyWireM.endTransmission();
}

void START_FEED(int LENGTH_OF_CUTS){
  if (LENGTH_OF_CUTS <= 0){
    SET_DIRECTION_BACKWARD();
    LENGTH_OF_CUTS = (LENGTH_OF_CUTS *( -1));
  }
  float STEPPS_NEEDED = LENGTH_OF_CUTS/CIRC_PER_STEP;
  if ((STEPPS_NEEDED-int(STEPPS_NEEDED)) >= .5){
    STEPPS_NEEDED = STEPPS_NEEDED + 1;
  }
  for (int x = 0; x < STEPPS_NEEDED; x++)
  {
    STEPPER_PULSE();
  }
  CUT_THE_WIRE();
}

void STEPPER_PULSE(){
  digitalWrite(STEPPER_MOTOR_PULSE_PIN, HIGH);   // sets the LED on
  delayMicroseconds(((STEPPER_MOTOR_STEPS_PER_REV*MICROSTEPPING_AMOUNT)/RPS_OF_MOTOR)*1000);
  digitalWrite(STEPPER_MOTOR_PULSE_PIN, LOW);    // sets the LED off
  delayMicroseconds(((STEPPER_MOTOR_STEPS_PER_REV*MICROSTEPPING_AMOUNT)/RPS_OF_MOTOR)*1000);
}  

void CUT_THE_WIRE(){
  TinyWireM.beginTransmission(LCD_I2C_SLAVE_ADDR);
  TinyWireM.send(5);
  TinyWireM.endTransmission();

}

void START_PROCESSING(int LENGTH_OF_CUTS,int NUMBER_OF_CUTS){
  int LENGTH_FED = 0;
  int CUTS_MADE = 0;
  float STEPPS_NEEDED = LENGTH_OF_CUTS/CIRC_PER_STEP;
  if ((STEPPS_NEEDED-int(STEPPS_NEEDED)) >= .5){
    STEPPS_NEEDED = STEPPS_NEEDED + 1;
  }
  for (int C = 0; C < NUMBER_OF_CUTS; C++){

    for (int P = 0; P < STEPPS_NEEDED; P++)
    {
      STEPPER_PULSE();
      LENGTH_FED = P*CIRC_PER_STEP;
      FEEDER_UPDATE(LENGTH_OF_CUTS,NUMBER_OF_CUTS,LENGTH_FED,C);
    }
    CUT_THE_WIRE();
    delay(2000);
  }
}


void ENABLE_STEPPER(){
  TinyWireM.beginTransmission(LCD_I2C_SLAVE_ADDR);
  TinyWireM.send(10);
  TinyWireM.endTransmission();
}

void DISABLE_STEPPER(){
  TinyWireM.beginTransmission(LCD_I2C_SLAVE_ADDR);
  TinyWireM.send(11);
  TinyWireM.endTransmission();
}

void SET_DIRECTION_FORWARD(){
  TinyWireM.beginTransmission(LCD_I2C_SLAVE_ADDR);
  TinyWireM.send(12);
  TinyWireM.endTransmission();
}

void SET_DIRECTION_BACKWARD(){
  TinyWireM.beginTransmission(LCD_I2C_SLAVE_ADDR);
  TinyWireM.send(13);
  TinyWireM.endTransmission();
}





