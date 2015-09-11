/*
* LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 */

#include <LiquidCrystal.h>
LiquidCrystal lcd(1,2,3,4,5,6);

#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {
    '1','2','3'                                                }
  ,
  {
    '4','5','6'                                                }
  ,
  {
    '7','8','9'                                                }
  ,
  {
    '#','0','*'                                                }
};
byte rowPins[ROWS] = {
  7, 8, 9, 10}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {
  11, 12, 13}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );


void setup(){

}

void loop (){
  LCD_MESSAGE_GET_LENGTH();
  int LENGTH_OF_CUTS = KEYPAD_VALUE();
  LCD_MESSAGE_GET_CUTS();
  int NUMBER_OF_CUTS = KEYPAD_VALUE();
  LCD_MESSAGE_WIRE_LENGTH_NEEDED(LENGTH_OF_CUTS,NUMBER_OF_CUTS);
  LCD.clear();
  LCD_FEED_AND_CUT_SCREEN(LENGTH_OF_CUTS,NUMBER_OF_CUTS,0,0);

}

void LCD_MESSAGE_GET_LENGTH(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("LENGTH OF WIRE: ");
  lcd.setCursor(5,1);
  lcd.print("INCH");
  return;
}

void LCD_MESSAGE_GET_CUTS(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("NUMBER OF CUTS: ");
  lcd.setCursor(5,1);
  lcd.print("CUTS");
  return;
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
  while(1){
    char key = keypad.getKey();
    if (key == '#') {
      return;
    }
    if (key == '*') { 
      loop();
    }
  }
}

void LCD_FEED_AND_CUT_SCREEN(int LENGTH_OF_CUTS,int NUMBER_OF_CUTS,int LENGTH_FED,int CUTS_MADE){
  lcd.home();
  LZ_LENGTH_OF_CUTS = LCD_LEADING_ZERO(LENGTH_OF_CUTS);
  LZ_NUMBER_OF_CUTS = LCD_LEADING_ZERO(NUMBER_OF_CUTS);
  LZ_LENGTH_FED = LCD_LEADING_ZERO(LENGTH_FED);
    LZ_CUTS_MADE = LCD_LEADING_ZERO(CUTS_MADE);
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

int KEYPAD_VALUE(){
  int x = 0;
  lcd.setCursor(x,1);
  lcd.cursor();
  lcd.blink();
  int value;

  char key[] = {
    'x','x','x','x','x'                                      };

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
        lcd.setCursor(x,1);
      lcd.print(key[x]);
      x++;
    }
  }
}

int KEYPAD_WIRES(){


}
























