
/*
  UDPSendReceive.pde:
 This sketch receives UDP message strings, prints them to the serial port
 and sends an "acknowledge" string back to the sender
 
 A Processing sketch is included at the end of file that can be used to send 
 and received messages for testing with a computer.
 
 created 21 Aug 2010
 by Michael Margolis
 
 This code is in the public domain.
 */


#include <SPI.h>         // needed for Arduino versions later than 0018
#include <Ethernet.h>
#include <EthernetUdp.h>         // UDP library from: bjoern@cs.stanford.edu 12/30/2008


#define UDP_TX_PACKET_MAX_SIZE 96

#define STATUS_LED 13
// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {  
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 0, 177);

unsigned int localPort = 8888;      // local port to listen on

// buffers for receiving and sending data
unsigned char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,
char ReplyBuffer[] = "lcd_states:";       // a string to send back
//char ReplyBuffer[32] = "lcd_states:";       // a string to send back

// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;

#define AMOUNT_LCDS 32

uint8_t LCDStates[AMOUNT_LCDS];
unsigned long lastInteraction = 0;

void setup() {
  // start the Ethernet and UDP:
  
  pinMode( STATUS_LED, OUTPUT);
      
  Ethernet.begin(mac,ip);
  Udp.begin(localPort);
  setupLCDs();
  Serial.begin(115200);
  
  Serial.println("VSE_Prototyping ready to go....");
}

void loop() {
  
  // if there's data available, read a packet
  boolean changed = processUDP();
  if( changed ){
    Serial.println("INPUT MODE");
    digitalWrite( STATUS_LED, HIGH);
    updateLCDs();
    lastInteraction = millis();
//    sendCurrentState();
  }
  
  if( lastInteraction + 5000 < millis() ){
    Serial.println("DEMO MODE");
    loopGBCModes();
  }
  
  delay(1);
  digitalWrite( STATUS_LED, LOW);
}





void sendCurrentState(){
  IPAddress remote = Udp.remoteIP();
  for (int i =0; i < 4; i++) {
    Serial.print(remote[i], DEC);
    if (i < 3) {
      Serial.print(".");
    }
  }
  Serial.print(", port ");
  Serial.println(Udp.remotePort());
  
  Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
  Udp.write(ReplyBuffer);  
//  Udp.write();
  uint8_t* states = getLCDStates();
  for( int i=0;i<32;i++ ){
//    Udp.write(states);
    Udp.write( states[i] );
//    
  }
  Udp.endPacket();
}  




boolean processUDP(){
  int packetSize = Udp.parsePacket();
  if( !packetSize ) return false;
    // read the packet into packetBufffer
  Udp.read(packetBuffer,UDP_TX_PACKET_MAX_SIZE);
  if( packetSize == 32 ){
    setLCDS( packetBuffer, 32 );
  }
  
  // send a reply, to the IP address and port that sent us the packet we received
  return true;
}


/*
  Processing sketch to run with this example
 =====================================================
 
 // Processing UDP example to send and receive string data from Arduino 
 // press any key to send the "Hello Arduino" message
 
 
 import hypermedia.net.*;
 
 UDP udp;  // define the UDP object
 
 
 void setup() {
 udp = new UDP( this, 6000 );  // create a new datagram connection on port 6000
 //udp.log( true ); 		// <-- printout the connection activity
 udp.listen( true );           // and wait for incoming message  
 }
 
 void draw()
 {
 }
 
 void keyPressed() {
 String ip       = "192.168.1.177";	// the remote IP address
 int port        = 8888;		// the destination port
 
 udp.send("Hello World", ip, port );   // the message to send
 
 }
 
 void receive( byte[] data ) { 			// <-- default handler
 //void receive( byte[] data, String ip, int port ) {	// <-- extended handler
 
 for(int i=0; i < data.length; i++) 
 print(char(data[i]));  
 println();   
 }
 */


