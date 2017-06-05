#include "ThingSpeak.h" // Communicate with ThingSpeak.com

// Import the private information
unsigned long ThingSpeakChannelID = CHANNELID;
const char * ThingSpeakWriteAPIKey = WRITEAPIKEY;

DateTime ThingSpeakTimer;
const unsigned long thingSpeakPostingInterval = 30; // seconds between ThingSpeak posts

void initializeThingSpeak() {
  writeLCD("Starting ThingSpeak...");
  Serial.println("Starting ThingSpeak client");
  ThingSpeak.begin(client); // Start the ThingSpeak connection
  ThingSpeakTimer = getRTCCurrentTime();
}

void postToThingSpeak(){
  ThingSpeak.writeFields(ThingSpeakChannelID, ThingSpeakWriteAPIKey);
}

void postDataToThingSpeak(float airTemp, float oilTemp, String flameStatus){
  DateTime now = getRTCCurrentTime();
  unsigned long timeSpan = now.unixtime() - ThingSpeakTimer.unixtime();
  if (timeSpan >= thingSpeakPostingInterval) {
    setThingSpeakField(1, airTemp);
    setThingSpeakField(2,oilTemp);
    setThingSpeakField(3,flameStatus);
    setThingSpeakStatus(flameStatus);
    postToThingSpeak();
    ThingSpeakTimer = now;
  }
}

void postThingSpeakStatus(String status){
  setThingSpeakStatus(status);
  postToThingSpeak();
}

void setThingSpeakStatus(String status){
  ThingSpeak.setStatus(status);
}

void setThingSpeakField(int field, float val) {
  ThingSpeak.setField(field,val);
}

void setThingSpeakField(int field, String val) {
  ThingSpeak.setField(field,val);
}
