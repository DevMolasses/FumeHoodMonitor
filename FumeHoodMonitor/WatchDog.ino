DateTime watchDogTimer;
const unsigned short watchDogPetDuration = 1; // length of watch dog pets in seconds; MUST BE LESS THAN INTERVAL
const unsigned short watchDogPetInterval = 5 - watchDogPetDuration; // seconds between of watch dog pets

#define watchDogPin 1

void initializeWatchDog(){
  watchDogTimer = getRTCCurrentTime();
  pinMode(watchDogPin, OUTPUT);
}

void petWatchDog(){
  DateTime now = getRTCCurrentTime();
  unsigned long timeSpan = now.unixtime() - watchDogTimer.unixtime();
  if (timeSpan >= watchDogPetInterval) {
    digitalWrite(watchDogPin, HIGH); // start petting
    if (timeSpan >= watchDogPetInterval + watchDogPetDuration) {
      digitalWrite(watchDogPin, LOW); // stop petting
      watchDogTimer = getRTCCurrentTime(); // reset timer
    }
  }
}
