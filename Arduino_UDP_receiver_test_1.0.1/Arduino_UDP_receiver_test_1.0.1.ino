/*
 * Final creater: YANG Mingcong
 * Version : 2020-04-08 V_1.0.0 Alpha
 * 
 * This version gives the standard timing function with hardware interruptin in Digital Pin 2
 * 
 * Using 1602A as displayer to display the timecode in the void loop() cause it did not need more precision
 * 
 * For the precision time,  part UDP package send, is directly be included in the Hardware Interrupt part
 */


//globle var
uint8_t g_frame = 0;
uint8_t g_second = 0;
uint8_t g_minute = 0;
uint8_t g_hour = 0;
uint8_t g_type = 1;

uint8_t loopCount = 0;


/*
  UDPReceive
 
 This sketch receives UDP message strings, prints them to the serial port
 and sends an "acknowledge" string back to the sender
 
 A Processing sketch is included at the end of file that can be used to send 
 and received messages for testing with a computer.
 
 created 21 Aug 2010
 by Michael Margolis
 
 This code is in the public domain.
 
 */


#include <SPI.h>          // needed for Arduino versions later than 0018
#include <Ethernet.h>
#include <EthernetUdp.h>  // UDP library from: bjoern@cs.stanford.edu 12/30/2008


// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {  
  0xEE, 0x01, 0x23, 0x45, 0x67, 0x88 };
IPAddress ip(192, 168, 3 ,208);

unsigned int localPort = 1936;              // local port to listen on

// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];  //buffer to hold incoming packet,

uint8_t packetBuffer2[UDP_TX_PACKET_MAX_SIZE];
// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;




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
  // start the Ethernet and UDP:
  Ethernet.begin(mac,ip);
  Udp.begin(localPort);




  // set up the LCD's number of columns and rows:
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.clear();
  // Print a message to the LCD.
  lcd.print("ArtNetTimeCode");

  //delay(2000); //delay 5 second to wait system stable
}


void loop() {
  //for Crystal Display 1602A
  showTC(g_hour,g_minute,g_second,g_frame);
  
  int packetSize = Udp.parsePacket();
  if(packetSize == 19)
  {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    //Serial.print("From ");
    IPAddress remote = Udp.remoteIP();
    /*
    for (int i =0; i < 4; i++)
    {
      Serial.print(remote[i], DEC);
      if (i < 3)
      {
        Serial.print(".");
      }
    }
    */
    //Serial.print(", port ");
    Serial.println(Udp.remotePort());

    // read the packet into packetBufffer
    Udp.read(packetBuffer2,UDP_TX_PACKET_MAX_SIZE);
    //Serial.println("Contents:");
    //for(int i=0;i<UDP_TX_PACKET_MAX_SIZE;i++){
    //Serial.println(packetBuffer2[i]);}

    g_frame = packetBuffer2[14];
    g_second = packetBuffer2[15];
    g_minute = packetBuffer2[16];
    g_hour = packetBuffer2[17];
  }
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


void UDP_TimeCode(uint8_t _h, uint8_t _m, uint8_t _s, uint8_t _f ,uint8_t _type){

    
    Udp.beginPacket({0xFF,0xFF,0xFF,0xFF}, 1936);

        uint8_t _0 = 0x00;

        uint8_t Data[19] = {'A','r','t','-','N','e','t', _0,
                                    _0, 0x97, _0, 14, _0, _0,
                                    _f, _s, _m, _h, _type };           //151 = 0x97
  
        for(int i = 0; i < 19; i++){     Udp.write(Data[i]);    }        
        
  Udp.endPacket();
  
  }


