#include "ThingSpeak.h" // Communicate with ThingSpeak.com
#include <SPI.h> // Library for SPI serial commuincations
#include <Ethernet2.h> // Library to use the ethernet feather wing
#include "Wire.h" // Library to use i2c comunications
#include <Adafruit_GFX.h> // Adafruit library for commuincating with led display
#include "Adafruit_LEDBackpack.h" // Adafruit library for using the i2c board on the display
#include "Adafruit_MCP9808.h" // Adafruit library for using the MCP9808 (ambient temp)
#include <Adafruit_MAX31865.h> // Adafruit library for using the MAX31865 (RTD Amp)
#include <NTPClient.h> // Library to access time from the NTP servers
#include "RTClib.h" // Library for accessing the Featherwing RTC on i2c 0x68
#include "private.h" // Private library to store private information

// Import in the private information
unsigned long ThingSpeakChannelID = CHANNELID;
const char * ThingSpeakWriteAPIKey = WRITEAPIKEY;
byte mac[] = MAC;

// Make all necessary pin and address assignments
#define WIZ_CS 10 // Defines the SPI clock pin for the Ethernet port
#define TCAADDR 0x72 // Address of the i2c multiplexer
#define flameSensorPin 6 // Digital pin to read the flame sensor
#define TMP36Pin A1 // Analog pin to read the TMP36 ambient temperature sensor

// Used to keep track of ThingSpeak posting intervals
unsigned long timer;
const unsigned long thingSpeakPostingInterval = 15000; // milliseconds between ThingSpeak posts

const int numReadings = 10;
int readings[numReadings];
int readIndex = 0;
int total = 0;
int average = 0;

EthernetClient client; // Instanciate the Ethernet Client
EthernetUDP Udp;
NTPClient timeClient(Udp);
RTC_PCF8523 rtc; // Instanciate the RTC

Adafruit_7segment tempDisplay = Adafruit_7segment();
Adafruit_7segment countDisplay = Adafruit_7segment();

// Create the MCP9808 temperature sensor object
Adafruit_MCP9808 tempsensor = Adafruit_MCP9808();

// Use software SPI: CS, DI, DO, CLK
Adafruit_MAX31865 max = Adafruit_MAX31865(10, 11, 12, 13);
// The value of the Rref resistor. Use 430.0!
#define RREF 430.0



long count = 0;

// Used to select which channel to use on the i2c multiplexer
void tcaselect(uint8_t i) {
  if (i > 7) return;

  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();
}

// Used to unselect all channels on the i2c multiplexer
void tcaunselect(){
  Wire.beginTransmission(TCAADDR);
  Wire.write(0);
  Wire.endTransmission();
}

float readTemp(){
  tcaunselect();
  tempsensor.shutdown_wake(0);
  float c = tempsensor.readTempC();
  float f = c * 9.0 / 5.0 + 32;
//  Serial.print("Temp: "); Serial.print(c); Serial.print("*C\t");
//  Serial.print(f); Serial.println("*F");
  delay(250);
  tempsensor.shutdown_wake(1);
  return f;
}

float readRTDTemp(){
  float c = max.temperature(100, RREF);
  float f = c * 9.0 / 5.0 + 32;
  return f;
}

float readTMP36Temp(){
  total -= readings[readIndex];
  readings[readIndex] = analogRead(TMP36Pin);
  total += readings[readIndex];
  readIndex++;
  if (readIndex >= numReadings)readIndex = 0;
  average = total / numReadings;
  float voltage = average * 3.3 / 1024;
//  voltage /= 1024.0;
  float tempC = (voltage - 0.5) * 100;
  float tempF = (tempC * 9.0 / 5.0) + 32.0;
  return tempF;
}

void displayTemp(float t){
  tcaselect(0);
  if (t>75) tempDisplay.blinkRate(1);
  else tempDisplay.blinkRate(0);
  tempDisplay.print(t);
  tempDisplay.writeDisplay();
}

void displayCount(float c){
  tcaselect(1);
  countDisplay.print(c);
  countDisplay.writeDisplay();
}

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  delay(1000);
//  Serial.println("\nHello! I am the Ethernet FeatherWing Fume Hood Monitor.");

  Ethernet.init(WIZ_CS); // Initialize the ethernet port
  delay(1000); // give the ethernet module time to boot up

  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    while(1);
  }

  Serial.println("Starting NTP CLient");
  timeClient.begin(); // Start the NTP time client
  delay(1000); // Give the client time to start up

  Serial.println("Starting ThingSpeak client");
  ThingSpeak.begin(client); // Start the ThingSpeak connection

  Serial.println("Starting Wire");
  Wire.begin(); // Start up the i2c

  Serial.println("Starting RTC");
  rtc.begin(); // Start up the RTC

  Serial.println("Starting the RTD");
  max.begin(MAX31865_3WIRE); // Start the RTD amplifier

  Adafruit_7segment displays[] = {tempDisplay, countDisplay};
  byte i2cAddr[] = {0x70, 0x70};
  uint8_t mpLoc[] = {0, 1};
  Serial.println("Starting Displays");
//  startDisplays(displays, i2cAddr, mpLoc, 2);
  tcaselect(0);
    tempDisplay.begin(0x70);
//    tempDisplay.print(0000);
//    tempDisplay.writeDisplay();

  tcaselect(1);
    countDisplay.begin(0x70);
//    countDisplay.print(333);
//    countDisplay.writeDisplay();

  for (int i = 1; i < 11; i++) {
    tcaselect(0);
    tempDisplay.print(i*1111);
    tempDisplay.writeDisplay();
    tcaselect(1);
    countDisplay.print(i*1111);
    countDisplay.writeDisplay();
    delay(250);
  }
  tcaunselect();
  Serial.println("Starting Temp Sensor");
  if (!tempsensor.begin()) {
    Serial.println("Couldn't find MCP9808!");
  while (1);
  }

  timeClient.update(); // Get the latest NTP time stamp
  rtc.adjust(DateTime(timeClient.getEpochTime())); // Update the RTC with the NTP time at boot to counteract the effects of float
  DateTime n = rtc.now();
  String statusMsg = "Monitor Rebooted at ";
  statusMsg += n.year();
  statusMsg += "/";
  if (n.month() < 10) statusMsg += "0";
  statusMsg += n.month();
  statusMsg += "/";
  if (n.day() < 10) statusMsg += "0";
  statusMsg += n.day();
  statusMsg += " ";
  if (n.hour() < 10) statusMsg += "0";
  statusMsg += n.hour();
  statusMsg += ":";
  if (n.minute() < 10) statusMsg += "0";
  statusMsg += n.minute();
  statusMsg += ":";
  if (n.second() < 10) statusMsg += "0";
  statusMsg += n.second();
  Serial.println("Sending ThingSpeak status: " + statusMsg);
  ThingSpeak.setStatus(statusMsg);
  ThingSpeak.writeFields(ThingSpeakChannelID, ThingSpeakWriteAPIKey);
  timer = millis();

  for (int thisReading = 0; thisReading < numReadings; thisReading++){
    readings[thisReading] = analogRead(TMP36Pin);
    total += readings[thisReading];
  }
}

void loop() {
  short flameValue = digitalRead(flameSensorPin);
  String flameMeaning;
  if (flameValue == 0) flameMeaning = String("On Fire");
  else flameMeaning = String("All Clear");
  float temp = readTemp();
  float rtdTemp = readRTDTemp();
  float TMP36 = readTMP36Temp();
  displayTemp(temp);
  count++;
  displayCount(TMP36);
  unsigned long now = millis();
  if (now - timer > 30000) {
    ThingSpeak.setField(1,temp);
    ThingSpeak.setField(2,rtdTemp);
    ThingSpeak.setField(3,flameMeaning);
    //ThingSpeak.setStatus(flameMeaning);
    ThingSpeak.writeFields(ThingSpeakChannelID, ThingSpeakWriteAPIKey);
    timer = now;
  }
//  delay(1000); // update every second
//  delay(20000); // ThingSpeak will only accept updates every 15 seconds.
}
