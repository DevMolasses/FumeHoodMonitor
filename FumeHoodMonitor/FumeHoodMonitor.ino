#include <SPI.h> // Library for SPI serial commuincations
#include "Wire.h" // Library to use i2c comunications
#include "private.h" // Private library to store private information
#include "RTClib.h" // Library for accessing the Featherwing RTC on i2c 0x68

// Used to keep track of ThingSpeak posting intervals
// This should probably change to use the RTC


void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  delay(1000);

  // Start Wire for i2c communications
  Serial.println("Starting Wire");
  Wire.begin(); // Start up the i2c
  initializeLCD();
  initializeEthernetPort();
  initializeRTC();
  initializeNTPClient();
  updateRTCToNTPTime();
  initializeWatchDog();
  initializeRTD(); // Oil temp
  initializeDS18B20(); // Digital air temp
  initializePowerRelay(); // Relay to control hot plate power
  initializeThingSpeak();
  String statusMsg = getRTCCurrentTimeString();
  statusMsg += " - Monitor Rebooted";
  postThingSpeakStatus(statusMsg);
  writeLCD("Monitor Rebooted at", getRTCCurrentTimeString());
}

void loop() {
  // Collect the data from the sensors
  String flameStatus = getFlameStatus();
  float oilTemp = readRTDTemp();
  float airTemp = readDS18B20Temp();

  writeLCD(getRTCCurrentTimeString(), airTemp, oilTemp, flameStatus);
  setLCDTemperatureColor();
  postDataToThingSpeak(airTemp, oilTemp, flameStatus);
  controlHotPlatePowerRelay(airTemp, oilTemp);
  setLCDTemperatureColor();
  petWatchDog();
}
