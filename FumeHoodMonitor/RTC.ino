#include <NTPClient.h> // Library to access time from the NTP servers

NTPClient timeClient(Udp);
RTC_PCF8523 rtc; // Instanciate the RTC

void initializeNTPClient() {
  Serial.println("Starting NTP CLient");
  timeClient.begin(); // Start the NTP time client
  delay(1000); // Give the client time to start up
}

void initializeRTC(){
  Serial.println("Starting RTC");
  rtc.begin(); // Start up the RTC
}

void updateRTCToNTPTime() {
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
