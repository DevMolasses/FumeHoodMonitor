#include "Adafruit_LiquidCrystal.h"

#define REDLITE A3
#define GREENLITE A4
#define BLUELITE A5

Adafruit_LiquidCrystal lcd(0);
int brightness = 255;

void initializeLCD(){
  lcd.begin(20,4);
  writeLCD("Starting LCD...");
  pinMode(REDLITE, OUTPUT);
  pinMode(GREENLITE, OUTPUT);
  pinMode(BLUELITE, OUTPUT);
  setBacklight(255,0,0);
  delay(1000);
  setBacklight(0,0,255);
  delay(1000);
  setBacklight(255,255,0);
  delay(1000);
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
  lcd.print("Fume Hood Monitor   ");
  lcd.setCursor(0,1);
  lcd.print("Ambient Temp: ");
  lcd.print(ambientTemp, 2);
  lcd.setCursor(0,2);
  lcd.print("Fluid Temp: ");
  lcd.print(fluidTemp, 2);
  lcd.setCursor(0,3);
  lcd.print("Flame: ");
  lcd.print(flameStatus);
}

void writeLCD(String timeString, float ambientTemp, float fluidTemp, String flameStatus){
  lcd.setCursor(0,0);
  lcd.print(make20Characters(timeString));
  lcd.setCursor(0,1);
  lcd.print(make20Characters("Ambient Temp: " + String(ambientTemp, 2)));
  lcd.setCursor(0,2);
  lcd.print(make20Characters("Fluid Temp: " + String(fluidTemp, 2)));
  lcd.setCursor(0,3);
  lcd.print(make20Characters("Flame: " + flameStatus));
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
