#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 19

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

const int numDS18B20Readings = 5;
const float maxDS18B20Temperature = 200.0; //Deg F
float DS18B20readings[numDS18B20Readings];
int DS18B20readIndex = 0;
float DS18B20total = 0;
float DS18B20average = 0;

void initializeDS18B20() {
  writeLCD("Starting DC18B20...");
  sensors.begin();

  for (int thisReading = 0; thisReading < numDS18B20Readings; thisReading++){
    sensors.requestTemperatures();
    DS18B20readings[thisReading] = sensors.getTempFByIndex(0);
    DS18B20total += DS18B20readings[thisReading];
  }
}

float readDS18B20Temp() {
  updateDS18B20Readings();
  return DS18B20average;
}

void updateDS18B20Readings(){
  sensors.requestTemperatures();
  DS18B20total -= DS18B20readings[DS18B20readIndex];
  DS18B20readings[DS18B20readIndex] = sensors.getTempFByIndex(0);
  DS18B20total += DS18B20readings[DS18B20readIndex];
  DS18B20readIndex++;
  if (DS18B20readIndex >= numDS18B20Readings)DS18B20readIndex = 0;
  DS18B20average = DS18B20total / numDS18B20Readings;
}
