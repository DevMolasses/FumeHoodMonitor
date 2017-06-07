#include "Adafruit_LiquidCrystal.h"

#define REDLITE 5
#define GREENLITE 6
#define BLUELITE A3
bool backlightBlinkState = true;
int backLightBlinkCycleCountLimit = 1;
int backlightBlinkCycleCount = 0;
Adafruit_LiquidCrystal lcd(0);
int brightness = 255;

void initializeLCD(){
  lcd.begin(20,4);
  writeLCD("Starting LCD...");
  Serial.println("Starting LCD");
  pinMode(REDLITE, OUTPUT);
  pinMode(GREENLITE, OUTPUT);
  pinMode(BLUELITE, OUTPUT);
  cycleBacklightColor();
  setBacklight("green");
}

void writeLCD(String message){
  lcd.clear();
  lcd.print(message);
}

void writeLCD(String message1, String message2){
  lcd.clear();
  lcd.print(message1);
  lcd.setCursor(0,1);
  lcd.print(message2);
}

void writeLCD(float ambientTemp, float fluidTemp, String flameStatus){
  lcd.setCursor(0,0);
  lcd.print(make20Characters("Fume Hood Monitor"));
  lcd.setCursor(0,1);
  lcd.print(make20Characters("Air Temp: " + String(ambientTemp, 2) + String((char)223) + "F"));
  lcd.setCursor(0,2);
  lcd.print(make20Characters("Fluid Temp: " + String(fluidTemp, 2) + String((char)223) + "F"));
  lcd.setCursor(0,3);
  lcd.print(make20Characters("Flame: " + flameStatus));
}

void writeLCD(String timeString, float ambientTemp, float fluidTemp, String flameStatus){
  lcd.setCursor(0,0);
  lcd.print(make20Characters(timeString));
  lcd.setCursor(0,1);
  lcd.print(make20Characters("Air Temp: " + String(ambientTemp, 2) + String((char)223) + "F"));
  lcd.setCursor(0,2);
  lcd.print(make20Characters("Fluid Temp: " + String(fluidTemp, 2) + String((char)223) + "F"));
  lcd.setCursor(0,3);
  lcd.print(make20Characters("Flame: " + flameStatus));
}

void setLCDTemperatureColor() {
  if (onFire) {
    if (backlightBlinkState) setBacklight("red");
    else setBacklight(0, 0, 0);
    if (backlightBlinkCycleCount >= backLightBlinkCycleCountLimit){
      backlightBlinkState = !backlightBlinkState;
      backlightBlinkCycleCount = 0;
    } else backlightBlinkCycleCount++;
  } else if (airHigh || oilHigh) setBacklight("red");
  else setBacklight("green");
}

String make20Characters(String str){
  for (unsigned int i = 0; i < (20 - str.length()); i++) str += " ";
  return str;
}

void setBacklight(uint8_t r, uint8_t g, uint8_t b) {

  // Adjust the values for the set brightness
  r = map(r, 0, 255, 0, brightness);
  g = map(g, 0, 255, 0, brightness);
  b = map(b, 0, 255, 0, brightness);

  // common anode so invert!
  r = map(r, 0, 255, 255, 0);
  g = map(g, 0, 255, 255, 0);
  b = map(b, 0, 255, 255, 0);

  // Set the pins
  analogWrite(REDLITE, r);
  analogWrite(GREENLITE, g);
  analogWrite(BLUELITE, b);
}

void setBacklight(String color) {
  uint8_t r, g, b;
  if      (color == "red")    {r = 255; g = 0;   b = 0;}
  else if (color == "orange") {r = 255; g = 50;  b = 0;}
  else if (color == "yellow") {r = 255; g = 255; b = 0;}
  else if (color == "green")  {r = 0;   g = 255; b = 0;}
  else if (color == "blue")   {r = 0;   g = 0;   b = 255;}
  else                        {r = 255; g = 255; b = 255;}
  setBacklight(r, g, b);
}

void cycleBacklightColor() {
  uint8_t d = 1;
  for (uint8_t i = 0; i < 255; i++) {
    setBacklight(0, i, 0);
    delay(d);
  }
  for (uint8_t i = 0; i < 255; i++) {
    setBacklight(0, 254, i);
    delay(d);
  }
  for (uint8_t i = 0; i < 255; i++) {
    setBacklight(0, 254 - i, 254);
    delay(d);
  }
  for (uint8_t i = 0; i < 255; i++) {
    setBacklight(i, 0, 254);
    delay(d);
  }
  for (uint8_t i = 0; i < 255; i++) {
    setBacklight(254, 0, 254 - i);
    delay(d);
  }
  for (uint8_t i = 0; i < 255; i++) {
    setBacklight(254, i, 0);
    delay(d);
  }
  for (uint8_t i = 0; i < 255; i++) {
    setBacklight(254 - i, 254, 0);
    delay(d);
  }
}
