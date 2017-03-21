#include "ThingSpeak.h" // Communicate with ThingSpeak.com

// Import the private information
unsigned long ThingSpeakChannelID = CHANNELID;
const char * ThingSpeakWriteAPIKey = WRITEAPIKEY;



void initializeThingSpeak() {
  Serial.println("Starting ThingSpeak client");
  ThingSpeak.begin(client); // Start the ThingSpeak connection
}

void postToThingSpeak(){
  ThingSpeak.writeFields(ThingSpeakChannelID, ThingSpeakWriteAPIKey);
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
