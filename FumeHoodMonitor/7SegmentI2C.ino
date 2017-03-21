#include <Adafruit_GFX.h> // Adafruit library for commuincating with led display
#include "Adafruit_LEDBackpack.h" // Adafruit library for using the i2c board on the display

Adafruit_7segment tempDisplay = Adafruit_7segment();
Adafruit_7segment countDisplay = Adafruit_7segment();

// void startDisplays(Adafruit_7segment displays[], byte i2cAddr[], uint8_t mpLoc[], int n){
//
//   // Start the displays
//   for (int i = 0; i < n; i++) {
//     Serial.println("mpLoc: " + mpLoc[i]);
//     tcaselect(mpLoc[i]);
//       Serial.println("i2sAddr: " + i2cAddr[i]);
//       displays[i].begin(i2cAddr[i]);
//   }
//
//   // Show some values changing on the displays
//   for (int i = 0; i < 10; i++) {
// //    String num;
// //    if (i == 0) num = "0000";
// //    else num = String(i*1111);
//     for (int j = 0; j < n; j++) {
//       tcaselect(mpLoc[j]);
//       for (int k = 0; k < 4; k++) displays[j].print(i);
//       displays[j].writeDisplay();
//     }
//     delay(200);
//   }
//
//   tcaunselect();
// }

void initialize7SegmentDisplays() {
  //Adafruit_7segment displays[] = {tempDisplay, countDisplay};
  // byte i2cAddr[] = {0x70, 0x70};
  // uint8_t mpLoc[] = {0, 1};
  Serial.println("Starting Displays");
//  startDisplays(displays, i2cAddr, mpLoc, 2);
  tcaselect(0);
    tempDisplay.begin(0x70);

  tcaselect(1);
    countDisplay.begin(0x70);

  for (int i = 1; i < 11; i++) {
    tcaselect(0);
    tempDisplay.print(i*1111);
    tempDisplay.writeDisplay();
    tcaselect(1);
    countDisplay.print(i*1111);
    countDisplay.writeDisplay();
    delay(250);
  }
  tcaunselect();
}

void displayTemp(float t){
  tcaselect(0);
  if (t>75) tempDisplay.blinkRate(1);
  else tempDisplay.blinkRate(0);
  tempDisplay.print(t);
  tempDisplay.writeDisplay();
}

void displayCount(float c){
  tcaselect(1);
  countDisplay.print(c);
  countDisplay.writeDisplay();
}
