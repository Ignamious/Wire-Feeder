//SLAVE

/*

 ATMEL ATTINY84 / ARDUINO
 
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
 
 6 PINS DISPLAY
 1 PIN SERVO
 
 7/10 USED
 
 */
#define MY_I2C_SLAVE_ADDR 2
#define STEPPER_MOTOR_ENABLE_PIN 0
#define STEPPER_MOTOR_REVERSE_PIN 1
#define SERVO_PIN_NUMBER 8
#include <LiquidCrystal.h>
LiquidCrystal lcd(1,2,3,4,5,6);
#include <TinyWireS.h>
#include <Servo.h>

Servo CUT_SERVO;

void setup(){
  TinyWireS.begin(MY_I2C_SLAVE_ADDR);
  CUT_SERVO.attach(SERVO_PIN_NUMBER);
  pinMode(STEPPER_MOTOR_ENABLE_PIN, OUTPUT);
  pinMode(STEPPER_MOTOR_REVERSE_PIN, OUTPUT);
}

void loop(){
  int RECIVED_INSTRUCTIONS = TinyWireS.receive();
  switch (RECIVED_INSTRUCTIONS){
  case 1:
    LCD_MESSAGE_GET_LENGTH();
    break;
  case 2:
    LCD_MESSAGE_GET_CUTS();
    break;
  case 3:
    LCD_MESSAGE_WIRE_LENGTH_NEEDED(TinyWireS.receive(),TinyWireS.receive());
    break;
  case 4:
    LCD_FEED_AND_CUT_SCREEN(TinyWireS.receive(),TinyWireS.receive(),TinyWireS.receive(),TinyWireS.receive());
    break;
  case 5:
    CUT_THE_WIRE();
    break;
  case 10:
    ENABLE_STEPPER();
    break;
  case 11:
    DISABLE_STEPPER();
    break;
  case 12:
    SET_DIRECTION_FORWARD();
    break;
  case 13:
    SET_DIRECTION_BACKWARD();
    break;
  case 20: //lcd.clear
    lcd.clear();
    break;
  case 21: //lcd.home
    lcd.home();
    break;
  case 22: //lcd.setCursor
    lcd.setCursor(TinyWireS.receive(),TinyWireS.receive());
    break;
  case 23: //lcd.cursor
    lcd.cursor();
    break;
  case 24: //lcd.noCursor
    lcd.noCursor();
    break;
  case 25: //lcd.blink
    lcd.blink();
    break;
  case 26: //lcd.noBlink
    lcd.noBlink();
    break;
  case 27: //lcd.print
    for (int x = 0; x <= TinyWireS.available(); x++){
      lcd.print(TinyWireS.receive());
    }
    break;
  case 28: //lcd.write
    for (int x = 0; x <= TinyWireS.available(); x++){
      lcd.write(TinyWireS.receive());
    }
    break;
  }
}

void LCD_MESSAGE_GET_LENGTH(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("LENGTH OF WIRE:");
  lcd.setCursor(5,1);
  lcd.print("INCH");
  loop();
}

void LCD_MESSAGE_GET_CUTS(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("NUMBER OF CUTS:");
  lcd.setCursor(5,1);
  lcd.print("CUTS");
  loop();
}

void  LCD_MESSAGE_WIRE_LENGTH_NEEDED(int LENGTH_OF_CUTS,int NUMBER_OF_CUTS){
  lcd.clear();
  long TOTAL_VALUE = (LENGTH_OF_CUTS * NUMBER_OF_CUTS);
  lcd.noCursor();
  lcd.noBlink();
  lcd.setCursor(0,0);
  lcd.print(TOTAL_VALUE);
  lcd.print(" INCHES");
  lcd.setCursor(0,1);
  lcd.print("OF WIRE NEEDED.");
  loop();
}

void LCD_FEED_AND_CUT_SCREEN(int LENGTH_OF_CUTS,int NUMBER_OF_CUTS,int LENGTH_FED,int CUTS_MADE){
  lcd.home();
  int CUTS_LEFT = NUMBER_OF_CUTS - CUTS_MADE;
  int LC_CUTS_LEFT = LCD_LEADING_ZERO(CUTS_LEFT);
  int LZ_LENGTH_OF_CUTS = LCD_LEADING_ZERO(LENGTH_OF_CUTS);
  int LZ_NUMBER_OF_CUTS = LCD_LEADING_ZERO(NUMBER_OF_CUTS);
  int  LZ_LENGTH_FED = LCD_LEADING_ZERO(LENGTH_FED);
  int LZ_CUTS_MADE = LCD_LEADING_ZERO(CUTS_MADE);

  LCD_PRINT_LZ(LZ_LENGTH_FED);
  lcd.print(LENGTH_FED);
  lcd.print(" OF ");
  LCD_PRINT_LZ(LZ_LENGTH_OF_CUTS);
  lcd.print(LENGTH_OF_CUTS);
  lcd.print(" IN.");
  lcd.setCursor(0,1);
  LCD_PRINT_LZ(LC_CUTS_LEFT);
  lcd.print(CUTS_LEFT);
  lcd.print(" CUTS LEFT");
}

int LCD_LEADING_ZERO(int x){
  int y;
  if (x<1000){
    y++;
  }
  if (x<100){
    y++;
  }
  if (x<10){
    y++;
  }

  return y;
}

void LCD_PRINT_LZ(int ZEROS){
  for (int x = 0; x < (ZEROS+1); x++){
    lcd.print(0); 
  }
}

void CUT_THE_WIRE(){
  CUT_SERVO.write(1);
  delay(1000);
  CUT_SERVO.write(89);
  delay(1000);
}

byte ENABLE_STEPPER(){
  digitalWrite(STEPPER_MOTOR_ENABLE_PIN, HIGH);
  return HIGH;
}

byte DISABLE_STEPPER(){
  digitalWrite(STEPPER_MOTOR_ENABLE_PIN, LOW);
  return LOW;
}

byte SET_DIRECTION_FORWARD(){
  digitalWrite(STEPPER_MOTOR_REVERSE_PIN, HIGH);
  return HIGH;
}

byte SET_DIRECTION_BACKWARD(){
  digitalWrite(STEPPER_MOTOR_REVERSE_PIN, LOW);
  return LOW;
}











