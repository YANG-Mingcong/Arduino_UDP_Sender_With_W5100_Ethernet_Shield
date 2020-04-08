/*
 * Final creater: YANG Mingcong
 * Version : 2020-04-08 V_1.2.0 Alpha
 * 
 * This version gives the standard timing function with hardware interruptin in Digital Pin 2
 * 
 */


//globle var
uint8_t g_frame = 0;
uint8_t g_second = 0;
uint8_t g_minute = 0;
uint8_t g_hour = 0;

uint8_t loopCount = 0;




/*
  LiquidCrystal Library - Hello World

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch prints "Hello World!" to the LCD
 and shows the time.

  The circuit:
 * LCD RS pin to digital pin 12       8
 * LCD Enable pin to digital pin 11   9
 * LCD D4 pin to digital pin 5        2
 * LCD D5 pin to digital pin 4        5
 * LCD D6 pin to digital pin 3        6
 * LCD D7 pin to digital pin 2        7
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
LiquidCrystal lcd(8,9,3,5,6,7);//lcd(12, 11, 5, 4, 3, 2);


/*
* author: Sebastian Wallin
* description:
* Example on how to configure the periodical execution of a user
* defined function (Interrupt service routine) using Timer2. This
* example will run the function every 1ms.
*
* For detailed information on Timer2 configuration see chapter 17 in
* ATMEGA328 datasheet.
*/

/* Timer2 reload value, globally available */
unsigned int tcnt2;

/* Toggle HIGH or LOW digital write */
int toggle = 0;

void setup() {
/* Configure the test pin as output */
  pinMode(2, OUTPUT);

   /* First disable the timer overflow interrupt while we're configuring */
  TIMSK2 &= ~(1<<TOIE2);

  /* Configure timer2 in normal mode (pure counting, no PWM etc.) */
  TCCR2A &= ~((1<<WGM21) | (1<<WGM20));
  TCCR2B &= ~(1<<WGM22);

  /* Select clock source: internal I/O clock */
  ASSR &= ~(1<<AS2);

  /* Disable Compare Match A interrupt enable (only want overflow) */
  TIMSK2 &= ~(1<<OCIE2A);

  /* Now configure the prescaler to CPU clock divided by 128 */
  TCCR2B |= (1<<CS22)  | (1<<CS20); // Set bits
  TCCR2B &= ~(1<<CS21);             // Clear bit

  /* We need to calculate a proper value to load the timer counter.
   * The following loads the value 131 into the Timer 2 counter register
   * The math behind this is:
   * (CPU frequency) / (prescaler value) = 125000 Hz = 8us. =》 16MHz / 
   * (desired period) / 8us = 125.
   * MAX(uint8) + 1 - 125 = 131;
   */
  /* Save value globally for later reload in ISR */
  tcnt2 = 131;
  //tcnt2 = 6; //for every 2 ms per circle
  /* Finally load end enable the timer */
  TCNT2 = tcnt2;
  TIMSK2 |= (1<<TOIE2);

  delay(1000);




  // set up the LCD's number of columns and rows:
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.clear();
  // Print a message to the LCD.
  lcd.print("ArtNetTimeCode");
}


/*
* Install the Interrupt Service Routine (ISR) for Timer2 overflow.
* This is normally done by writing the address of the ISR in the
* interrupt vector table but conveniently done by using ISR()  */
ISR(TIMER2_OVF_vect) {
  /* Reload the timer */
  TCNT2 = tcnt2;
  /* Write to a digital pin so that we can confirm our timer */
  digitalWrite(2, toggle == 0 ? HIGH : LOW);
  toggle = ~toggle;

  //loopCount +=1;
  //loopCount = (loopCount) % 40;
  //goLoop();
  
  loopCount +=1;
  if(0 == loopCount%40){
      loopCount = 0;
      timerCount();
    }
}





void loop() {
    //lcd.rightToLeft();
    lcd.setCursor(2, 1);
    
      // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);


  //showTC(1,23,sec,06);
  showTC(g_hour,g_minute,g_second,g_frame);
  Serial.println(g_hour);


  

}

void timerCount()
{
          if(25==g_frame){
            g_frame = 0;
            //g_second +=1;

                          if(60 == g_second){
                          g_second = 0;
                          //g_minute +=1;
              
                                    if(60 == g_minute){
                                        g_minute = 0;
                                        //g_hour +=1;
                              
                                                if(24 == g_hour){
                                                      g_hour = 0;
                                                    }else{
                                                      g_hour += 1;
                                                    } 
                                        
                                      }else{
                                        g_minute += 1;
                                      }
                        
              
              
              
                          
                        }else{
                          g_second += 1;
                        }

            
          }else{
            g_frame += 1;
          }
          
         
          
          
          
  
  }






void showTC(uint8_t _h, uint8_t _m, uint8_t _s, uint8_t _f){
    lcd.setCursor(0, 1);

    
    //Format all int into 2 characts String
    //String Data = "00:00:00.00 -25f";

    String Data = intToString(_h) + ":"
                + intToString(_m) + ":" 
                + intToString(_s) + "."
                + intToString(_f) + " -25";
    
      
    lcd.print(Data);
    
    
  }

String intToString(uint8_t _int){
  String _result;
  if(_int<10){ 
      _result = "0" + (String)_int;
    }else{
      _result = (String)_int;
    }

  return _result;
  }
