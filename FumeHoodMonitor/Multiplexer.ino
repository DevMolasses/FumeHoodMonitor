#define TCAADDR 0x72 // Address of the i2c multiplexer

// Used to select which channel to use on the i2c multiplexer
void tcaselect(uint8_t i) {
  if (i > 7) return;

  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();
}

// Used to unselect all channels on the i2c multiplexer
void tcaunselect(){
  Wire.beginTransmission(TCAADDR);
  Wire.write(0);
  Wire.endTransmission();
}
