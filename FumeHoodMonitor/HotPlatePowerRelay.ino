#define relayPin 18

bool onFire = false;
bool airHigh = false;
bool oilHigh = false;

void initializePowerRelay(){
  writeLCD("Starting PWR Relay..");
  Serial.println("Starting Hot Plate Power Relay");
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH);
}

void powerUpHotPlate(){
  digitalWrite(relayPin, LOW);
}

void powerDownHotPlate(){
  digitalWrite(relayPin, HIGH);
}

void controlHotPlatePowerRelay(float airTemp, float oilTemp){
  if (isFlamePresent() || isAirTempOnFire(airTemp) || isOilTempOnFire(oilTemp)) onFire = true;
  else {
    if (isOilTempHigh(oilTemp)) oilHigh = true;
    else if (isOilTempLow(oilTemp)) oilHigh = false;
    if (isAirTempHigh(airTemp)) airHigh = true;
    else if (isAirTempLow(airTemp)) airHigh = false;
  }
  if(onFire || oilHigh || airHigh) powerDownHotPlate();
  else powerUpHotPlate();
}
