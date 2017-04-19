#include <Adafruit_MAX31865.h> // Adafruit library for using the MAX31865 (RTD Amp)
const int numRTDReadings = 10;
const float maxRTDTemperature = 350.0; //Deg F
float RTDreadings[numRTDReadings];
int RTDreadIndex = 0;
int RTDtotal = 0;
int RTDaverage = 0;

// Use software SPI: CS, DI, DO, CLK
Adafruit_MAX31865 rtd = Adafruit_MAX31865(10, 11, 12, 13);

// The value of the Rref resistor. Use 430.0!
#define RREF 430.0

void initializeRTD(){
  Serial.println("Starting the RTD");
  rtd.begin(MAX31865_3WIRE); // Start the RTD amplifier

  for (int thisReading = 0; thisReading < numRTDReadings; thisReading++){
    RTDreadings[thisReading] = rtd.temperature(100, RREF);
    RTDtotal += RTDreadings[thisReading];
  }

}

float readRTDTemp(){
  // float c = rtd.temperature(100, RREF);
  // float f = c * 9.0 / 5.0 + 32;
  // return f;

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

bool isRTDTempHigh() {
  return readRTDTemp() > maxRTDTemperature;
}

bool isRTDTempHigh(float tempF){
  return tempF > maxRTDTemperature;
}
