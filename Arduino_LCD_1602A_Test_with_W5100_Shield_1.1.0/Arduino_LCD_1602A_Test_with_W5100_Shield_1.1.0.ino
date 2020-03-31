/*
  LiquidCrystal Library - Hello World

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch prints "Hello World!" to the LCD
 and shows the time.

  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystal
 */

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8,9,2,5,6,7);//lcd(12, 11, 5, 4, 3, 2);



void setup() {
  // put your setup code here, to run once:

  // set up the LCD's number of columns and rows:
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.clear();
  // Print a message to the LCD.
  lcd.print("ArtNetTimeCode");
}


void loop() {
    //lcd.rightToLeft();
    lcd.setCursor(2, 1);
    
      // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  int sec = millis() / 1000;

  showTC(1,23,sec,06);

  

}

void showTC(int _h, int _m, int _s, int _f){
    lcd.setCursor(0, 1);

    
    //Format all int into 2 characts String
    //String Data = "00:00:00.00 -25f";

    String Data = intToString(_h) + ":"
                + intToString(_m) + ":" 
                + intToString(_s) + "."
                + intToString(_f) + " -25f";
    
      
    lcd.print(Data);
    
    
  }

String intToString(int _int){
  String _result;
  if(_int<10){ 
      _result = "0" + (String)_int;
    }else{
      _result = (String)_int;
    }

  return _result;
  }
