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
 * update note: 1.2.2
 * change the position of countLoop +=1 after send package
 * change the 25-60-60-24 to 24-59-59-23 in timeCount part
 * result 57sec for 60 sec real time.
 * 
 * 
 */


//globle var
uint8_t g_frame = 0;
uint8_t g_second = 0;
uint8_t g_minute = 0;
uint8_t g_hour = 0;
uint8_t g_type = 1;

uint8_t loopCount = 0;


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

unsigned int localPort = 1936;              // local port to listen on

// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];  //buffer to hold incoming packet,
char  ReplyBuffer[] = "acknowledged";       // a string to send back

// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;




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
  
  if(0 == loopCount%40){
      loopCount = 0;
      timerCount();
        Udp.beginPacket({0xFF,0xFF,0xFF,0xFF}, 1936);

        uint8_t _0 = 0x00;

        uint8_t Data[19] = {'A','r','t','-','N','e','t', _0,
                                    _0, 0x97, _0, 14, _0, _0,
                                    g_frame, g_second, g_minute, g_hour, g_type };           //151 = 0x97
  
        for(int i = 0; i < 19; i++){     Udp.write(Data[i]);    }        
        
        Udp.endPacket();
        
    }
      loopCount +=1;

}


void loop() {
  //for Crystal Display 1602A
  
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



