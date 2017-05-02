#define flameSensorPin 6 // Digital pin to read the flame sensor

String getFlameStatus(){
  short flameValue = readFlameSensor();
  String flameStatus;
  if (flameValue == 0) {
    flameStatus = String("On Fire");
  }
  else flameStatus = String("All Clear");
  return flameStatus;
}

bool isFlamePresent(){
  return readFlameSensor() == 0;
}

short readFlameSensor(){
  return digitalRead(flameSensorPin);
}
