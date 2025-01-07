//Failed


/*
  UDPSendReceive
 
 This sketch receives UDP message strings, prints them to the serial port
 and sends an "acknowledge" string back to the sender
 
 A Processing sketch is included at the end of file that can be used to send 
 and received messages for testing with a computer.
 
 created 21 Aug 2010
 by Michael Margolis
 
 This code is in the public domain.
 
 */


long long totalFrames = 0; //max is one hour, cause every one hour micros will goes 0,


#include <SPI.h>          // needed for Arduino versions later than 0018
#include <Ethernet.h>
#include <EthernetUdp.h>  // UDP library from: bjoern@cs.stanford.edu 12/30/2008


// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {  
  0xEE, 0x01, 0x23, 0x45, 0x67, 0x89 };
IPAddress ip(192, 168, 3 ,209);

unsigned int localPort = 1936;              // local port to listen on

// buffers for receiving and sending data
//char packetBuffer[UDP_TX_PACKET_MAX_SIZE];  //buffer to hold incoming packet,
//char  ReplyBuffer[] = "acknowledged";       // a string to send back

// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;

//int second_tmp = 0;

uint8_t loopCount = 0;

    uint8_t m_h = 0;
    uint8_t m_m = 0;
    uint8_t m_s = 0;
    uint8_t m_f = 0;
    uint8_t m_type = 1; //default type = 1 means 25 fps



//globle var
int g_frame = 0;
int g_second = 0;
int g_minute = 0;
int g_hour = 0;



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

/* Setup phase: configure and enable timer2 overflow interrupt */
void setup() {
  /* Configure the test pin as output */
  //pinMode(2, OUTPUT);

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





    //old Setup

  delay(5000); //delay 5 second to wait system stable
  // start the Ethernet and UDP:
  Ethernet.begin(mac,ip);
  Udp.begin(localPort);

  Serial.begin(115000);


  // set up the LCD's number of columns and rows:
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

  loopCount +=1;
  //loopCount = (loopCount) % 40;
  goLoop();
}

/* Main loop. Empty, but needed to avoid linker errors */




void loop() {
  
}

void goLoop()
{
  if(loopCount%40 == 0){
        Serial.println(loopCount);
 
      
        //IPAddress broad_Cast(255,255,255,255);
        /**
        int m_frame = totalFrames % 25;
        int m_second =  (totalFrames / 25) % 60;
        int m_minute = (totalFrames / 1500) % 60;
        int m_hour = (totalFrames / 36000) % 24;
        */
        //UDP_TimeCode(g_hour,g_minute,g_second,g_frame,1);
        showTC(g_hour,g_minute,g_second,g_frame);
        
        if(25==g_frame){
            g_frame = 0;
            g_second +=1;
          }
          else{
            g_frame += 1;
          }
         if(60 == g_second){
            g_second = 0;
            g_minute +=1;
          }
          else{
            g_second += 1;
          }
          if(60 == g_minute){
            g_minute = 0;
            g_hour +=1;
          }
          else{
            g_minute += 1;
          }
          if(24 == g_hour){
            g_hour = 0;
          }
          else{
            g_hour += 1;
          }
        
        
        
        
        //UDP_TimeCode(m_hour,m_minute,m_second,m_frame,1);
        //delay(40);
        //totalFrames += 1;

        
  }
}

void UDP_TimeCode(int _h, int _m, int _s, int _f ,int _type){

    
    Udp.beginPacket({0xFF,0xFF,0xFF,0xFF}, 1936);

        uint8_t _0 = 0x00;
    
    m_h = _h;
    m_m = _m;
    m_s = _s;
    m_f = _f;
    m_type = _type;

        uint8_t Data[19] = {'A','r','t','-','N','e','t', _0,
                                    _0, 0x97, _0, 14, _0, _0,
                                    m_f, m_s, m_m, m_h, m_type };           //151 = 0x97
  
        for(int i = 0; i < 19; i++){     Udp.write(Data[i]);    }        
        
  Udp.endPacket();
  
  }


void showTC(int _h, int _m, int _s, int _f){
    //lcd.setCursor(0, 1);

    
    //Format all int into 2 characts String
    //String Data = "00:00:00.00 -25f";

    String Data = intToString(_h) + ":"
                + intToString(_m) + ":" 
                + intToString(_s) + "."
                + intToString(_f) + " -25f";

    //lcd.print(Data);   
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
