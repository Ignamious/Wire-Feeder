/* @file HelloKeypad.pde
 || @version 1.0
 || @author Alexander Brevig
 || @contact alexanderbrevig@gmail.com
 ||
 || @description
 || | Demonstrates the simplest use of the matrix Keypad library.
 || #
 */
#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {
    '1','2','3'                                                                }
  ,
  {
    '4','5','6'                                                                }
  ,
  {
    '7','8','9'                                                                }
  ,
  {
    '*','0','#'                                                                }
};
/*
Pin 1 = COL 2 = B4 = 24
 Pin 2 = Row 1 = B3 = 23
 Pin 3 = COL 1 = B2 = 22
 Pin 4 = Row 4 = B1 = 21
 Pin 5 = COL 3 = B0 = 20
 Pin 6 = Row 3 = E7 = 19
 Pin 7 = Row 2 = E6 = 18
 
 */

#include <LiquidCrystal.h>

byte rowPins[ROWS] = {
  23, 18, 19, 21}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {
  22, 24, 20 }; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup(){
  Serial.begin(9600);
  lcd.begin(20,4);


}

void loop(){

  int wire = NEW_KEYPAD_VALUE();
  lcd.print(wire);
  while(1){

  }
}


int NEW_KEYPAD_VALUE(){
  int LENGTH_OF_WIRE = 0;
  lcd.print("Length of cut:");
  lcd.setCursor(5,1);
  lcd.print("Inches");
  int x = 0;
  int y = 3;
  lcd.setCursor(x,1);
  lcd.cursor();
  lcd.blink();
  char key[] = {
    '0','0','0','0'                };
  while(1){
    key[y] = keypad.getKey();
    lcd.setCursor(x,1);
    if (key[y] != NO_KEY && key[y] != '#' && key[y] != '*'){
      lcd.print(key[y]);
      Serial.print("Place \"");
      Serial.print(x);
      Serial.print("\" is = ");
      Serial.println(key[y]);
      x++;
      y--;
    }
    if (key[y] == '*') {
      lcd.clear();
      NEW_KEYPAD_VALUE();
    }
    if (x > 4){
      Serial.println("too many variables, resetting x");
      x = 0;
      y = 4;
    }
    if (key[y] =='#') {
      lcd.clear();
      Serial.println("Calculating");
      x = x - 1;
      Serial.println(x);
      int z = x;
      for (int y = 0, y <= x, y++){
        LENGTH_OF_WIRE += FIND_ME(key[z],y);
        z--;
        Serial.println(LENGTH_OF_WIRE);
      }

    }
    //   ((int_key[0]*1000)+(int_key[1]*100)+(int_key[2]*10)+int_key[3]);
    //   Serial.println(LENGTH_OF_WIRE);
    //  break;
    //        return LENGTH_OF_WIRE;

  }

}

int FIND_ME(char KEY, int x){
 int the_return = 0;
the_return += key*(pow(10*x); 
return the_return;
}





























