#include <Adafruit_MAX31865.h> // Adafruit library for using the MAX31865 (RTD Amp)

// Use software SPI: CS, DI, DO, CLK
Adafruit_MAX31865 rtd = Adafruit_MAX31865(10, 11, 12, 13);

// The value of the Rref resistor. Use 430.0!
#define RREF 430.0

void initializeRTD(){
  Serial.println("Starting the RTD");
  rtd.begin(MAX31865_3WIRE); // Start the RTD amplifier
}

float readRTDTemp(){
  float c = rtd.temperature(100, RREF);
  float f = c * 9.0 / 5.0 + 32;
  return f;
}
