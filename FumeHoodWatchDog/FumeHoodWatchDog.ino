// This is a watchdog program to be run on an Adafruit Trinket

#define watchDogPin 1
#define resetPin 2

unsigned long watchDogInterval = 7000;
unsigned long resetDuration = 5000;
unsigned long fumeHoodMonitorBootTime = 15000;
unsigned long watchDogTimer;

bool watchDogHasBeenPet = false;

void setup() {
  pinMode(watchDogPin, INPUT);
  pinMode(resetPin, OUTPUT);
  digitalWrite(resetPin, HIGH);
  delay(fumeHoodMonitorBootTime);
  watchDogTimer = millis();
}

void loop(){
  watchDogHasBeenPet = digitalRead(watchDogPin) == 1;
  if (watchDogHasBeenPet) watchDogTimer = millis();
  unsigned long now = millis();
  if(now - watchDogTimer >= watchDogInterval){
    digitalWrite(resetPin, LOW);
    delay(resetDuration);
    digitalWrite(resetPin, HIGH);
    delay(fumeHoodMonitorBootTime);
    watchDogTimer = millis();
  }
}
