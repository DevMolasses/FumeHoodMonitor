#define flameSensorPin 6 // Digital pin to read the flame sensor

String getFlameStatus(){
  short flameValue = digitalRead(flameSensorPin);
  String flameMeaning;
  if (flameValue == 0) flameMeaning = String("On Fire");
  else flameMeaning = String("All Clear");
  return flameMeaning;
}
