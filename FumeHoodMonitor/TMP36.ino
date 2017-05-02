#define TMP36Pin A1 // Analog pin to read the TMP36 ambient temperature sensor
const int numReadings = 10;
const float maxTMP36Temperature = 200.0; //Deg F
int readings[numReadings];
int readIndex = 0;
int total = 0;
int average = 0;

// Gets a reading from the TMP36 sensor and stores it in the array for averaging
float readTMP36Temp(){
  updateReadings();
  float voltage = average * 3.3 / 1024;
  float tempC = (voltage - 0.5) * 100;
  float tempF = (tempC * 9.0 / 5.0) + 32.0;
  return tempF;
}

void updateReadings(){
  total -= readings[readIndex];
  readings[readIndex] = analogRead(TMP36Pin);
  total += readings[readIndex];
  readIndex++;
  if (readIndex >= numReadings)readIndex = 0;
  average = total / numReadings;
}
//Initializes the TMP36 sesor array by filling it with temperature readings
void initializeTMP36(){
  for (int thisReading = 0; thisReading < numReadings; thisReading++){
    readings[thisReading] = analogRead(TMP36Pin);
    total += readings[thisReading];
  }
}

bool isTempHigh(){
  return readTMP36Temp() > maxTMP36Temperature;
}

bool isTempHigh(float tempF){
  return tempF > maxTMP36Temperature;
}
