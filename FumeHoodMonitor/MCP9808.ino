#include "Adafruit_MCP9808.h" // Adafruit library for using the MCP9808 (ambient temp)

// Create the MCP9808 temperature sensor object
Adafruit_MCP9808 tempsensor = Adafruit_MCP9808();

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

void initializeMP9808() {
  Serial.println("Starting Temp Sensor");
  if (!tempsensor.begin()) {
    Serial.println("Couldn't find MCP9808!");
  while (1);
  }
}
