#include <Adafruit_MAX31865.h> // Adafruit library for using the MAX31865 (RTD Amp)
const int numRTDReadings = 10;
const float maxRTDTemperature = 350.0; //Deg F
float RTDreadings[numRTDReadings];
int RTDreadIndex = 0;
float RTDtotal = 0;
float RTDaverage = 0;

// Use software SPI: CS, DI, DO, CLK
Adafruit_MAX31865 rtd = Adafruit_MAX31865(10, 11, 12, 13);

// The value of the Rref resistor. Use 430.0!
#define RREF 430.0

void initializeRTD(){
  writeLCD("Starting RTD...");
  Serial.println("Starting the RTD");
  rtd.begin(MAX31865_3WIRE); // Start the RTD amplifier

  for (int thisReading = 0; thisReading < numRTDReadings; thisReading++){
    RTDreadings[thisReading] = rtd.temperature(100, RREF);
    RTDtotal += RTDreadings[thisReading];
  }
  Serial.println("RTD Started");
}

float readRTDTemp(){
  RTDtotal -= RTDreadings[RTDreadIndex];
  RTDreadings[RTDreadIndex] = rtd.temperature(100, RREF);
  RTDtotal += RTDreadings[RTDreadIndex];
  RTDreadIndex++;
  if (RTDreadIndex >= numRTDReadings)RTDreadIndex = 0;
  RTDaverage = RTDtotal / numRTDReadings;
  float tempC = RTDaverage;
  float tempF = (tempC * 9.0 / 5.0) + 32.0;
  return tempF;
}

bool isFluidTempHigh() {
  return readRTDTemp() > maxRTDTemperature;
}

bool isFluidTempHigh(float tempF){
  return tempF > maxRTDTemperature;
}
