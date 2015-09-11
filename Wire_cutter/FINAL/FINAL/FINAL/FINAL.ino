/*
When the MCU is first installed into the board before powering up the unit, Press and hold the "0" key and continue to hold it when you power up the unit.
 This will enter the config mode and will prompt you for all the Config values for your specific device.
 
 By holding the 1-5 you cna enter in the values seperitly.
 
 the "#" key is your "return / Next" Key
 the "*" Key is the back / reset key.
 
 When the unit is running, there is no way to stop the unit, unless you power it down.
 
 at this time there is no detection for the wire. make sure your total cut is under the amount of free wire you have.
 
 // ATMEL ATMEGA & 168 / ARDUINO
 //
 //                  +-\/-+
 //            PC6  1|    |28  PC5 (AI 5)
 //      (D 0) PD0  2|    |27  PC4 (AI 4)
 //      (D 1) PD1  3|    |26  PC3 (AI 3)
 //      (D 2) PD2  4|    |25  PC2 (AI 2)
 // PWM  (D 3) PD3  5|    |24  PC1 (AI 1)
 //      (D 4) PD4  6|    |23  PC0 (AI 0)
 //            VCC  7|    |22  GND
 //            GND  8|    |21  AREF
 //            PB6  9|    |20  AVCC
 //            PB7 10|    |19  PB5 (D 13)
 // PWM  (D 5) PD5 11|    |18  PB4 (D 12)
 // PWM  (D 6) PD6 12|    |17  PB3 (D 11) PWM
 //      (D 7) PD7 13|    |16  PB2 (D 10) PWM
 //      (D 8) PB0 14|    |15  PB1 (D 9)  PWM
 //                  +----+
 
 18 of 20 w/ crystal
 
 7 for keypad
 6 for LCD
 4 for stepper motor
 1 for servo
 
 */






#include <Keypad.h>
#include <LiquidCrystal.h>
#include <Servo.h>
#include <EEPROM.h>
#include <AccelStepper.h>
//These are the libraries that are needed to make the system work.


float STEPPER_MOTOR_STEPS_PER_REV;
float DIAMETER_OF_WIRE_FEED;
float MICROSTEPPING_AMOUNT;
float RPS_OF_MOTOR;
unsigned int WIRE_PER_SPOOL;
//Will make these into options that you can change. These wil be programmed into the EEPROM so they are saved between power cycles.

//#define STEPPER_MOTOR_PULSE_PIN 1
//#define STEPPER_MOTOR_ENABLE_PIN
//#define STEPPER_MOTOR_REVERSE_PIN 3
#define SERVO_PIN_NUMBER 16
//Pins used for the motors


Servo CUT_SERVO;

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {
    '1','2','3'                                                                                                                                                                                                                  }
  ,
  {
    '4','5','6'                                                                                                                                                                                                                  }
  ,
  {
    '7','8','9'                                                                                                                                                                                                                  }
  ,
  {
    '.','0','#'                                                                                                                                                                                                                  }
};

/******************************************************************************/
/*************************PIN DECLERATIONS*************************************/
byte rowPins[ROWS] = {
  23, 18, 19, 21}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {
  22, 24, 20 }; //connect to the column pinouts of the keypad



LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
/******************************************************************************/
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

float CIRC_PER_STEP;
AccelStepper stepper(4,7,8,9,10);

void setup(){
  lcd.begin(16,2);
  lcd.home();
  lcd.print("Hello.");
  delay(2000);
  READ_FROM_EEPROM();
  stepper.setMaxSpeed(RPS_OF_MOTOR);
  stepper.setAcceleration(100);
  CUT_SERVO.attach(SERVO_PIN_NUMBER);
  Serial.begin(9600);
  Serial.print("Foot per spool: ");
  Serial.println(readInt(1));
  Serial.print("DIAM OF WHEEL ");
  Serial.println(readFloat(20),5);
  Serial.print("MICROSTEP ");
  Serial.println(readFloat(30));
  Serial.print("STEP PER REV: ");
  Serial.println(readFloat(10));
  Serial.print("SPEED WANTED ");
  Serial.println(readFloat(40));
  Serial.print("PI: ");
  Serial.println(PI,8);
  Serial.print("PI*Diam: ");
  Serial.println(PI*readFloat(20),8);
    Serial.print("PI*Diam/stepps: ");
  Serial.println((PI*readFloat(20))/readFloat(10),8);
  //  pinMode(STEPPER_MOTOR_PULSE_PIN, OUTPUT);
  //  pinMode(STEPPER_MOTOR_ENABLE_PIN, OUTPUT);
  //  pinMode(STEPPER_MOTOR_REVERSE_PIN, OUTPUT);
  
  CIRC_PER_STEP = ((2*PI*(DIAMETER_OF_WIRE_FEED/2))/(STEPPER_MOTOR_STEPS_PER_REV));
  //  CUT_SERVO.write(170);
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
  for (int x = 0; x < (ZEROS); x++){
    lcd.print(0); 
  }
}

void loop(){

  switch (keypad.getKey()) {
  case '1':
    OPTION_SPOOL();
    RESTART_ME();
    break;
  case '2':
    OPTION_DIAMETER_OF_WIRE_FEED();
    RESTART_ME();
    break;
  case '3':
    OPTION_MICROSTEPPING_AMOUNT();
    RESTART_ME();
    break;
  case '4':
    OPTION_STEPPER_MOTOR_STEPS_PER_REV();
    RESTART_ME();
    break;
  case '5':
    OPTION_RPS_OF_MOTOR();
    RESTART_ME();
    break;
  case '6':
    break;
  case '7':
    break;
  case '8':
    break;
  case '9':
    break;
  case '0':
    OPTION_SPOOL();
    OPTION_DIAMETER_OF_WIRE_FEED();
    OPTION_MICROSTEPPING_AMOUNT();
    OPTION_STEPPER_MOTOR_STEPS_PER_REV();
    OPTION_RPS_OF_MOTOR();
    RESTART_ME();
    break;
  case '.':
    break;
  case '#':
    break;
  }

  LCD_MESSAGE_GET_LENGTH();
  int LENGTH_OF_CUTS = KEYPAD_VALUE();
  LCD_MESSAGE_GET_CUTS();
  int NUMBER_OF_CUTS = KEYPAD_VALUE();
  LCD_MESSAGE_WIRE_LENGTH_NEEDED(LENGTH_OF_CUTS, NUMBER_OF_CUTS);
  lcd.clear();
  FEEDER_UPDATE(LENGTH_OF_CUTS,NUMBER_OF_CUTS,0,0);
  ENABLE_STEPPER(); 
  START_PROCESSING(3,1);
  START_PROCESSING(LENGTH_OF_CUTS,NUMBER_OF_CUTS);
  START_PROCESSING(3,0);
  stepper.disableOutputs();
}

void RESTART_ME(){
  lcd.clear();
  lcd.home();
  lcd.print("Please RESET me ");
  lcd.setCursor(0,1);
  lcd.print("turn me OFF!");
  lcd.noCursor();
  lcd.noBlink();
  while (1){
    delay(500);
  }

}

void  LCD_MESSAGE_WIRE_LENGTH_NEEDED(int LENGTH_OF_CUTS,int NUMBER_OF_CUTS){
  lcd.clear();
  long TOTAL_VALUE = (LENGTH_OF_CUTS * NUMBER_OF_CUTS);
  lcd.noCursor();
  lcd.noBlink();
  lcd.setCursor(0,0);
  lcd.print(TOTAL_VALUE);
  lcd.print("IN. (");
  lcd.print(TOTAL_VALUE/12);
  lcd.print("FT)");
  lcd.setCursor(0,1);
  lcd.print("OF WIRE NEEDED.");
  while(1){
    int x = keypad.getKey();
    if (x == '#'){
      return; 
    }
    if (x == '.'){
      loop();
    }
  }
}

void LCD_MESSAGE_GET_CUTS(){

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("NUMBER OF CUTS:");
  lcd.setCursor(5,1);
  lcd.print("CUTS");
}

void LCD_MESSAGE_GET_LENGTH(){

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("LENGTH OF WIRE:");
  lcd.setCursor(5,1);
  lcd.print("INCH");
}

int KEYPAD_VALUE(){
  int x = 0;
  lcd.setCursor(x,1);
  lcd.cursor();
  lcd.blink();
  int value;

  char key[8];
  while (1){
    key[x] = keypad.getKey();
    if (key[x] != NO_KEY){
      if (key[x] == '.'){
        lcd.clear();
        loop();
      }
      else if (key[x] == '#'){
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
        case 5:
          value = (((key[0]-48)*10000)+((key[0]-48)*1000)+((key[1]-48)*100)+((key[2]-48)*10)+(key[3]-48));
          break;
        }
        return value;
      }
      else
        lcd.setCursor(x,1);
      lcd.print(key[x]);
      x++;
    }
  }
}



void FEEDER_UPDATE(int LENGTH_OF_CUTS,int NUMBER_OF_CUTS,int LENGTH_FED,int CUTS_MADE){
  lcd.setCursor(0,0);
  int CUTS_LEFT = NUMBER_OF_CUTS - CUTS_MADE;
  int LC_CUTS_LEFT = LCD_LEADING_ZERO(CUTS_LEFT);
  int LZ_LENGTH_OF_CUTS = LCD_LEADING_ZERO(LENGTH_OF_CUTS);
  int LZ_NUMBER_OF_CUTS = LCD_LEADING_ZERO(NUMBER_OF_CUTS);
  int  LZ_LENGTH_FED = LCD_LEADING_ZERO(LENGTH_FED);
  int LZ_CUTS_MADE = LCD_LEADING_ZERO(CUTS_MADE);
  // lcd.clear();
  // lcd.home();
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



/*

 void START_FEED(float LENGTH_OF_CUTS){
 if (LENGTH_OF_CUTS <= 0){
 SET_DIRECTION_BACKWARD();
 LENGTH_OF_CUTS = (LENGTH_OF_CUTS *( -1));
 }
 float STEPPS_NEEDED = float(LENGTH_OF_CUTS)/CIRC_PER_STEP;
 if ((STEPPS_NEEDED-int(STEPPS_NEEDED)) >= .5){
 STEPPS_NEEDED = int(STEPPS_NEEDED) + 1;
 }
 myStepper.step(STEPPS_NEEDED);
 //  delayMicroseconds(((STEPPER_MOTOR_STEPS_PER_REV*MICROSTEPPING_AMOUNT)/RPS_OF_MOTOR)*100);
 CUT_THE_WIRE();
 }
 */

/*
void STEPPER_PULSE(){
 digitalWrite(STEPPER_MOTOR_PULSE_PIN, HIGH);   // sets the LED on
 delayMicroseconds(((STEPPER_MOTOR_STEPS_PER_REV*MICROSTEPPING_AMOUNT)/RPS_OF_MOTOR)*100);
 digitalWrite(STEPPER_MOTOR_PULSE_PIN, LOW);    // sets the LED off
 delayMicroseconds(((STEPPER_MOTOR_STEPS_PER_REV*MICROSTEPPING_AMOUNT)/RPS_OF_MOTOR)*100);
 }  
 */

void CUT_THE_WIRE(){
  CUT_SERVO.write(10);
  delay(1000);
  CUT_SERVO.write(170);
  delay(1000);

}

void START_PROCESSING(int LENGTH_OF_CUTS,int NUMBER_OF_CUTS){
  int STEPPS_NEEDED;
  Serial.print("int LENGTH_OF_CUTS: ");
  Serial.println(LENGTH_OF_CUTS);
  Serial.print("int NUMBER_OF_CUTS: ");
  Serial.println(NUMBER_OF_CUTS);
  Serial.print("float CIRC_PER_STEP: ");
  Serial.println(CIRC_PER_STEP);
  float F_STEPPS_NEEDED = (float(LENGTH_OF_CUTS)/CIRC_PER_STEP);
  Serial.print("float STEPPS_NEEDED: ");
  Serial.println(F_STEPPS_NEEDED,8);
  if ((F_STEPPS_NEEDED - int(F_STEPPS_NEEDED)) >= .5)   STEPPS_NEEDED = int(F_STEPPS_NEEDED)+1; 
  else  STEPPS_NEEDED = int(F_STEPPS_NEEDED);

  Serial.print("int STEPPS_NEEDED: ");
  Serial.println(STEPPS_NEEDED);

  for (int c = 0; c < NUMBER_OF_CUTS; c++){
    stepper.setCurrentPosition(0);
    stepper.moveTo(STEPPS_NEEDED);
    while (stepper.currentPosition() <= STEPPS_NEEDED){ 
    stepper.run();
    FEEDER_UPDATE(LENGTH_OF_CUTS, NUMBER_OF_CUTS, stepper.currentPosition()*CIRC_PER_STEP, c);
    if (stepper.currentPosition() == STEPPS_NEEDED) break;
    }
    stepper.stop();
  //   stepper.runToNewPosition(0);
    CUT_THE_WIRE();
    FEEDER_UPDATE(LENGTH_OF_CUTS, NUMBER_OF_CUTS, 0, c);
  }


}
/*

 void START_PROCESSING(int LENGTH_OF_CUTS,int NUMBER_OF_CUTS){
 int LENGTH_FED = 0;
 int CUTS_MADE = 0;
 int I_STEPPS_NEEDED;
 float STEPPS_NEEDED = LENGTH_OF_CUTS/CIRC_PER_STEP;
 if ((STEPPS_NEEDED-int(STEPPS_NEEDED)) >= .5){
 I_STEPPS_NEEDED = int(STEPPS_NEEDED) + 1;
 }
 for (int a = 0; a<= NUMBER_OF_CUTS; a++){
 int my_count = STEPPS_NEEDED;
 int my_length = LENGTH_OF_CUTS;
 while(my_count >= 0){
 
 myStepper.step(20);
 my_length = my_length - (my_count*CIRC_PER_STEP);
 FEEDER_UPDATE(LENGTH_OF_CUTS,NUMBER_OF_CUTS,my_length,a);
 
 my_count = my_count - 20;
 }
 CUT_THE_WIRE();
 }
/*
 for (int a = 0; a<= NUMBER_OF_CUTS; a++){
 for (int b = 0; b < I_STEPPS_NEEDED; b++){
 myStepper.step((I_STEPPS_NEEDED/(LENGTH_OF_CUTS*4)));
 LENGTH_FED = (b*CIRC_PER_STEP);
 FEEDER_UPDATE(LENGTH_OF_CUTS,NUMBER_OF_CUTS,LENGTH_FED,a);
 }
 CUT_THE_WIRE();
 
 }
/*
 for (int C = 0; C < NUMBER_OF_CUTS; C++){
 
 for (int P = 0; P < STEPPS_NEEDED/LENGTH_OF_CUTS; P++)
 {
 myStepper.step(STEPPS_NEEDED/LENGTH_OF_CUTS);
 LENGTH_FED = ((P*STEPPS_NEEDED)*CIRC_PER_STEP);
 FEEDER_UPDATE(LENGTH_OF_CUTS,NUMBER_OF_CUTS,LENGTH_FED,C);
 //  delayMicroseconds(((STEPPER_MOTOR_STEPS_PER_REV*MICROSTEPPING_AMOUNT)/RPS_OF_MOTOR)*100);
 
 }
 CUT_THE_WIRE();
 delay(2000);
 }
 
 
 }
 
 */

void ENABLE_STEPPER(){
  //  digitalWrite(STEPPER_MOTOR_ENABLE_PIN, HIGH);
}

void DISABLE_STEPPER(){
  //  digitalWrite(STEPPER_MOTOR_ENABLE_PIN, LOW);
}

void SET_DIRECTION_FORWARD(){
  //  digitalWrite(STEPPER_MOTOR_REVERSE_PIN, HIGH);
}

void SET_DIRECTION_BACKWARD(){
  //  digitalWrite(STEPPER_MOTOR_REVERSE_PIN, LOW);
}

void OPTION_SPOOL(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.println("1. FT per Spool:");
  lcd.setCursor(0,1);
  lcd.blink();
  lcd.cursor();
  int SPOOL_LENGTH = KEYPAD_VALUE();
  writeInt(1, SPOOL_LENGTH);
}

void OPTION_DIAMETER_OF_WIRE_FEED(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.println("2. DIAM OF WHEEL");
  lcd.setCursor(0,1);
  lcd.blink();
  lcd.cursor();
  float SPOOL_LENGTH = KEYPAD_VALUE();
  for (byte x = 0;x < 7;x++){
    if (SPOOL_LENGTH > 10){
      Serial.print("Before:");
      Serial.println(SPOOL_LENGTH,8);
      SPOOL_LENGTH /= 10; 
      Serial.print("After:");
      Serial.println(SPOOL_LENGTH,8);
    }
  }
  writeFloat(20, SPOOL_LENGTH);
}

void OPTION_MICROSTEPPING_AMOUNT(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.println("3. MICROSTEP");
  lcd.setCursor(0,1);
  lcd.blink();
  lcd.cursor();
  float SPOOL_LENGTH = KEYPAD_VALUE();
  writeFloat(30, SPOOL_LENGTH);


}

void OPTION_STEPPER_MOTOR_STEPS_PER_REV(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.println("4. STEP PER REV");
  lcd.setCursor(0,1);
  lcd.blink();
  lcd.cursor();
  float SPOOL_LENGTH = KEYPAD_VALUE();

}

void OPTION_RPS_OF_MOTOR(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.println("5. RPM WANTED");
  lcd.setCursor(6,1);
  lcd.println("RPM(1-200)");
  lcd.setCursor(0,1);
  lcd.blink();
  lcd.cursor();
  float SPOOL_LENGTH = KEYPAD_VALUE();
  writeFloat(40, SPOOL_LENGTH);
}

void READ_FROM_EEPROM(){
  WIRE_PER_SPOOL = readInt(1);
  STEPPER_MOTOR_STEPS_PER_REV = readFloat(10);
  DIAMETER_OF_WIRE_FEED = readFloat(20);
  MICROSTEPPING_AMOUNT = readFloat(30);
  RPS_OF_MOTOR = readFloat(40);
  //Will make these into options that you can change. These wil be programmed into the EEPROM so they are saved between power cycles.
}


float readFloat(unsigned int addr){

  union READ_FLOAT
  {
    byte b[4];
    float f;
  }
  READ_FLOAT_V;

  for(int i = 0;i < 4;i++)
  {
    READ_FLOAT_V.b[i] = EEPROM.read(addr+i);
  }
  return READ_FLOAT_V.f;
}

void writeFloat(unsigned int addr, float x){
  union WRITE_FLOAT
  {
    byte b[4];
    float f;
  }
  WRITE_FLOAT_V;

  WRITE_FLOAT_V.f = x;
  for(int i = 0;i < 4;i++){
    EEPROM.write(addr+i, WRITE_FLOAT_V.b[i]);
  }
}

int readInt(unsigned int addr){
  union READ_INT
  {
    byte b[4];
    int in;
  }
  READ_INT_V;

  for(int i = 0;i < 4;i++)
  {
    READ_INT_V.b[i] = EEPROM.read(addr+i);
  }
  return READ_INT_V.in;
}

void writeInt(unsigned int addr, int x){
  union WRITE_INT
  {
    byte b[4];
    int in;
  }
  WRITE_INT_V;

  WRITE_INT_V.in = x;
  for(int i = 0;i < 4;i++){
    EEPROM.write(addr+i, WRITE_INT_V.b[i]);
  }
}













































