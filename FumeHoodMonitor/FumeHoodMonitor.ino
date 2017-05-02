#include <SPI.h> // Library for SPI serial commuincations
#include "Wire.h" // Library to use i2c comunications
#include "private.h" // Private library to store private information
#include "RTClib.h" // Library for accessing the Featherwing RTC on i2c 0x68

// Used to keep track of ThingSpeak posting intervals
// This should probably change to use the RTC
DateTime timer;
const unsigned long thingSpeakPostingInterval = 30; // seconds between ThingSpeak posts

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  delay(1000);
  // Start Wire for i2c communications
  Serial.println("Starting Wire");
  Wire.begin(); // Start up the i2c
  initializeLCD();
  writeLCD("Starting Ethernet...");
  initializeEthernetPort();
  writeLCD("Starting RTC...");
  initializeRTC();
  writeLCD("Starting NTP...");
  initializeNTPClient();
  writeLCD("Starting ThingSpeak...");
  initializeThingSpeak();
  writeLCD("Starting RTD...");
  initializeRTD();
  // initialize7SegmentDisplays();
  // initializeMP9808();
  writeLCD("Sync RTC to NTP...");
  updateRTCToNTPTime();
  writeLCD("Starting TMP36...");
  initializeTMP36();


  String statusMsg = "Monitor Rebooted at ";
  writeLCD(statusMsg, getRTCCurrentTimeString());

  statusMsg += getRTCCurrentTimeString();
  Serial.println("Sending ThingSpeak status: " + statusMsg);

  setThingSpeakStatus(statusMsg);
  postToThingSpeak();

  timer = getRTCCurrentTime();
}

void loop() {
  // Collect the data from the sensors
  String flameStatus = getFlameStatus();
  // bool flamePresent = isFlamePresent();

  // float temp = readTemp();
  float rtdTemp = readRTDTemp();
  float TMP36 = readTMP36Temp();
  // float TMP36 = readTMP36Voltage();
  // float TMP36 = getReadingAverage();
  writeLCD(getRTCCurrentTimeString(), TMP36, rtdTemp, flameStatus);

  // // Set the display
  // // displayTemp(temp);
  // // displayCount(TMP36);
  //
  // // Log data to ThingSpeak
  // DateTime now = getRTCCurrentTime();
  // unsigned long timeSpan = now.unixtime() - timer.unixtime();
  // if (timeSpan >= thingSpeakPostingInterval) {
  //   setThingSpeakField(1,temp);
  //   setThingSpeakField(2,rtdTemp);
  //   setThingSpeakField(3,flameStatus);
  //   setThingSpeakStatus(flameStatus);
  //   postToThingSpeak();
  //   timer = now;
  // }
}
