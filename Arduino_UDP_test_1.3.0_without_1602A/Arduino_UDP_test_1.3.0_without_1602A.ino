/*
 * Final creater: YANG Mingcong
 * Version : 2020-04-08 V_1.2.2 Alpha -b version with out 1602
 * 
 * This version gives the standard timing function with hardware interruptin in Digital Pin 2
 * 
 * Using 1602A as displayer to display the timecode in the void loop() cause it did not need more precision
 * 
 * For the precision time,  part UDP package send, is directly be included in the Hardware Interrupt part
 * 
 * 2nd tested just remove the lcd display function
 * result 55sec for 60 sec real time. same result
 * 
 * update note: 1.3.0
 * change the position of countLoop +=1 after send package
 * change the 25-60-60-24 to 24-59-59-23 in timeCount part
 * result 57sec for 60 sec real time.
 * 
 * add cli and sei,
 * 
 * change to timer1 for account
 * 
 */


//globle var
uint8_t g_frame = 0;
uint8_t g_second = 0;
uint8_t g_minute = 0;
uint8_t g_hour = 0;
uint8_t g_type = 1;

uint8_t _0 = 0x00;



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


#include <SPI.h>          // needed for Arduino versions later than 0018
#include <Ethernet.h>
#include <EthernetUdp.h>  // UDP library from: bjoern@cs.stanford.edu 12/30/2008


// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {  
  0xEE, 0x01, 0x23, 0x45, 0x67, 0x89 };
IPAddress ip(192, 168, 3 ,209);

// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;



/* Toggle HIGH or LOW digital write */
int toggle = 0;
boolean toggle1 = 0;

void setup() {
   pinMode(9, OUTPUT);

  
//https://www.instructables.com/id/Arduino-Timer-Interrupts/

cli();//stop interrupts

//set timer1 interrupt at 25Hz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 25hz increments
  OCR1A = 624;// = (16*10^6) / (25*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS10 and CS12 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  
sei();//allow interrupts

//END of Interrupts



  Serial.begin(115200);
  
  // start the Ethernet and UDP:
  Ethernet.begin(mac,ip);
  Udp.begin(1936);

  

}

ISR(TIMER1_COMPA_vect) {

//generates pulse wave of frequency 1Hz/2 = 0.5kHz (takes two cycles for full wave- toggle high then toggle low)
  
  //Serial.println(g_frame);
  timerCount();
  udpSend();
  

      

}

void loop() {
  //udpSend();
}

void udpSend()
{
  Udp.beginPacket({0xFF,0xFF,0xFF,0xFF}, 1936);

        Udp.write("Art-Net",8 );

        Udp.write(_0);
        Udp.write("\x97");
        
        Udp.write(_0);
        Udp.write("\x0e");
        
        Udp.write(_0);
        Udp.write(_0);
        
        Udp.write(g_frame);
        Udp.write(g_second);
        Udp.write(g_minute);
        Udp.write(g_hour);
        Udp.write(g_type);
      
  Udp.endPacket();
  }

void timerCount()
{
  
          if(24==g_frame){
            g_frame = 0;
            //g_second +=1;

                          if(59 == g_second){
                          g_second = 0;
                          //g_minute +=1;
              
                                    if(59 == g_minute){
                                        g_minute = 0;
                                        //g_hour +=1;
                              
                                                if(23 == g_hour){
                                                      g_hour = 0;
                                                    }else{
                                                      ++g_hour;
                                                    } 
                                        
                                      }else{
                                        ++g_minute;
                                      }
                        
              
                          
                        }else{
                          ++g_second;
                        }

            
          }else{
            ++g_frame;
          }
          
  }



