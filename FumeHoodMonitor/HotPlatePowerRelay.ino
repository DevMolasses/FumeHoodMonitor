#define relayPin 18

void initializePowerRelay(){
  writeLCD("Starting PWR Relay..");
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);
}

void powerUpHotPlate(){
  digitalWrite(relayPin, HIGH);
}

void powerDownHotPlate(){
  digitalWrite(relayPin, LOW);
}
