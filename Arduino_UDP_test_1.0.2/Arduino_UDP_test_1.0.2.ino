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
int totalHour = 0; 

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

int second_tmp = 0;

void setup() {
  // start the Ethernet and UDP:
  Ethernet.begin(mac,ip);
  Udp.begin(localPort);

  //Serial.begin(115000);
}

void loop() {

  
  // if there's data available, read a packet
  /***
  int packetSize = Udp.parsePacket();
  if(packetSize)
  {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remote = Udp.remoteIP();
    for (int i =0; i < 4; i++)
    {
      Serial.print(remote[i], DEC);
      if (i < 3)
      {
        Serial.print(".");
      }
    }
    Serial.print(", port ");
    Serial.println(Udp.remotePort());

    // read the packet into packetBufffer
    Udp.read(packetBuffer,UDP_TX_PACKET_MAX_SIZE);
    Serial.println("Contents:");
    Serial.println(packetBuffer);

    // send a reply, to the IP address and port that sent us the packet we received
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(ReplyBuffer);
    Udp.endPacket();
    
  }*/

  //IPAddress broad_Cast(255,255,255,255);
  int m_frame = totalFrames % 25;
  int m_second =  (totalFrames / 25) % 60;
  int m_minute = (totalFrames / 1500) % 60;
  int m_hour = (totalFrames / 36000) % 24;
  
  UDP_TimeCode(m_hour,m_minute,m_second,m_frame,1);
  delay(40);
  totalFrames += 1;
}

void UDP_TimeCode(int _h, int _m, int _s, int _f ,int _type){
    uint8_t m_h = 0;
    uint8_t m_m = 0;
    uint8_t m_s = 0;
    uint8_t m_f = 0;
    uint8_t m_type = 1; //default type = 1 means 25 fps
    
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
