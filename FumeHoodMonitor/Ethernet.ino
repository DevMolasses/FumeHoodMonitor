#include <Ethernet2.h> // Library to use the ethernet feather wing
EthernetClient client; // Instanciate the Ethernet Client
EthernetUDP Udp;

#define WIZ_CS 10 // Defines the SPI clock pin for the Ethernet port

byte mac[] = MAC;

void initializeEthernetPort() {
  Ethernet.init(WIZ_CS); // Initialize the ethernet port
  delay(1000); // give the ethernet module time to boot up

  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    while(1);
  }
}
