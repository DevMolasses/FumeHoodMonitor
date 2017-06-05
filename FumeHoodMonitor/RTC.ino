#include <NTPClient.h> // Library to access time from the NTP servers

NTPClient timeClient(Udp);
RTC_PCF8523 rtc; // Instanciate the RTC

void initializeNTPClient() {
  writeLCD("Starting NTP...");
  Serial.println("Starting NTP CLient");
  timeClient.begin(); // Start the NTP time client
  delay(1000); // Give the client time to start up
}

void initializeRTC(){
  writeLCD("Starting RTC...");
  Serial.println("Starting RTC");
  rtc.begin(); // Start up the RTC
}

void updateRTCToNTPTime() {
  writeLCD("Sync RTC to NTP...");
  timeClient.update(); // Get the latest NTP time stamp
  rtc.adjust(DateTime(timeClient.getEpochTime())); // Update the RTC with the NTP time at boot to counteract the effects of float
}

String getRTCCurrentTimeString(){
  DateTime currentTime = rtc.now();

  String currentTimeString = "";
  currentTimeString += currentTime.year();
  currentTimeString += "/";
  if (currentTime.month() < 10) currentTimeString += "0";
  currentTimeString += currentTime.month();
  currentTimeString += "/";
  if (currentTime.day() < 10) currentTimeString += "0";
  currentTimeString += currentTime.day();
  currentTimeString += " ";
  if (currentTime.hour() < 10) currentTimeString += "0";
  currentTimeString += currentTime.hour();
  currentTimeString += ":";
  if (currentTime.minute() < 10) currentTimeString += "0";
  currentTimeString += currentTime.minute();
  currentTimeString += ":";
  if (currentTime.second() < 10) currentTimeString += "0";
  currentTimeString += currentTime.second();
  return currentTimeString;
}

DateTime getRTCCurrentTime(){
 return rtc.now();
}

void TimeZoneAdjustedTimeString() {
  DateTime currentTime = rtc.now();

  if (currentTime.month() >= 4 && currentTime.month() <= 10) {
    //Adjust for Daylight Savings -5
  } else if (currentTime.month() == 3) { // DST starts on second sunday in March
    short sundayCount = 0;
    for (short i = currentTime.day(); i > 0; i--){
      DateTime cycleTime = DateTime(currentTime.year(), currentTime.month(), i, currentTime.hour(), currentTime.minute(), currentTime.second());
      if(cycleTime.dayOfTheWeek() == 0) sundayCount++;
    }
    if (sundayCount >= 2) {
      //adjust for DST -5
    } else {
      //adjust for STD -6
    }
  } else if (currentTime.month() == 11) { // DST ends on the first sunday of November
    short sundayCount = 0;
    for (short i = currentTime.day(); i > 0; i--){
      DateTime cycleTime = DateTime(currentTime.year(), currentTime.month(), i, currentTime.hour(), currentTime.minute(), currentTime.second());
      if(cycleTime.dayOfTheWeek() == 0) sundayCount++;
    }
    if (sundayCount >= 2) {
      //adjust for DST -5
    } else {
      //adjust for STD -6
    }
  } else {
    // Adjust for Standard Time -6
  }
  if (currentTime.month() >= 3 && currentTime.month() <= 11){
    if (currentTime.month() >= 4 && currentTime.month() <= 10){
      //adjust for DST -5
    } else if (currentTime.month() == 3) {

      // if (currentTime.day() >= 14){
      //   //adjust for DST -5
      // } else if (currentTime.day() < 8) {
      //   //adjust for STD -6
      // } else {
      //
      // }
    }
  } else {
    Serial.println("adjust for DST -5");
  }
}
