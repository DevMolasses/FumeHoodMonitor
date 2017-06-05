// This is a watchdog program to be run on an Adafruit Trinket

#define watchDogPin 2
#define resetPin 3
#define hbPin 1

unsigned long watchDogInterval = 30000; // Needs to see a pulse at lease every 30 seconds
unsigned long resetDuration = 5000;
unsigned long fumeHoodMonitorBootTime = 15000;
volatile unsigned long watchDogTimer;
volatile bool hbState = false;
// bool watchDogIsBeingPet = false;
// bool watchDogHasBeenPet = false;

void setup() {
  pinMode(watchDogPin, INPUT);
  attachInterrupt(0, petWatchDog, RISING);
  pinMode(resetPin, OUTPUT);
  pinMode(hbPin, OUTPUT);
  digitalWrite(resetPin, HIGH);
  digitalWrite(hbPin, hbState);
  delay(fumeHoodMonitorBootTime);
  watchDogTimer = millis();
}

void loop(){
  // watchDogIsBeingPet = digitalRead(watchDogPin) == 1;
  // if (watchDogIsBeingPet) {
  //   watchDogHasBeenPet = true;
  // } else if (!watchDogIsBeingPet && watchDogHasBeenPet) {
  //   watchDogTimer = millis();
  //   watchDogHasBeenPet = false;
  // }
  unsigned long now = millis();
  if(now - watchDogTimer >= watchDogInterval){
    digitalWrite(resetPin, LOW);
    delay(resetDuration);
    digitalWrite(resetPin, HIGH);
    delay(fumeHoodMonitorBootTime);
    watchDogTimer = millis();
  }
  digitalWrite(hbPin, hbState);
}

void petWatchDog(){
  watchDogTimer = millis();
  hbState = !hbState;
}
