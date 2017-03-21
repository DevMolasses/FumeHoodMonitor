#include <SPI.h> // Library for SPI serial commuincations
#include "Wire.h" // Library to use i2c comunications
#include "private.h" // Private library to store private information
#include "RTClib.h" // Library for accessing the Featherwing RTC on i2c 0x68

// Used to keep track of ThingSpeak posting intervals
// This should probably change to use the RTC
unsigned long timer;
const unsigned long thingSpeakPostingInterval = 15000; // milliseconds between ThingSpeak posts

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  delay(1000);
  // Start Wire for i2c communications
  Serial.println("Starting Wire");
  Wire.begin(); // Start up the i2c

  initializeEthernetPort();
  initializeRTC();
  initializeNTPClient();
  initializeThingSpeak();
  initializeRTD();
  initialize7SegmentDisplays();
  initializeMP9808();
  updateRTCToNTPTime();
  initializeTMP36();

  String statusMsg = "Monitor Rebooted at ";
  statusMsg += getRTCCurrentTimeString();
  Serial.println("Sending ThingSpeak status: " + statusMsg);

  setThingSpeakStatus(statusMsg);
  postToThingSpeak();

  timer = millis();
}

void loop() {
  String flameStatus = getFlameStatus();
  float temp = readTemp();
  float rtdTemp = readRTDTemp();
  float TMP36 = readTMP36Temp();
  // displayTemp(temp);
  displayCount(TMP36);
  unsigned long now = millis();
  if (now - timer > 30000) {
    setThingSpeakField(1,temp);
    setThingSpeakField(2,rtdTemp);
    setThingSpeakField(3,flameStatus);
    setThingSpeakStatus(flameStatus);
    postToThingSpeak();
    timer = now;
  }
}
