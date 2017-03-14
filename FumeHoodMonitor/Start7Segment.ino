void startDisplays(Adafruit_7segment displays[], byte i2cAddr[], uint8_t mpLoc[], int n){

  // Start the displays
  for (int i = 0; i < n; i++) {
    Serial.println("mpLoc: " + mpLoc[i]);
    tcaselect(mpLoc[i]);
      Serial.println("i2sAddr: " + i2cAddr[i]);
      displays[i].begin(i2cAddr[i]);
  }

  // Show some values changing on the displays
  for (int i = 0; i < 10; i++) {
//    String num;
//    if (i == 0) num = "0000";
//    else num = String(i*1111);
    for (int j = 0; j < n; j++) {
      tcaselect(mpLoc[j]);
      for (int k = 0; k < 4; k++) displays[j].print(i);
      displays[j].writeDisplay();
    }
    delay(200);
  }

  tcaunselect();
}
