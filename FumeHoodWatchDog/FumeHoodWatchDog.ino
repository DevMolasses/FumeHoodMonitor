// This is a watchdog program to be run on an Adafruit Trinket

#define watchDogPin 13
#define resetPin 2

unsigned long watchDogInterval = 30000; // Needs to see a pulse at lease every 30 seconds
unsigned long resetDuration = 5000;
unsigned long fumeHoodMonitorBootTime = 15000;
unsigned long watchDogTimer;

bool watchDogIsBeingPet = false;
bool watchDogHasBeenPet = false;

void setup() {
  Serial.begin(9600);
  pinMode(watchDogPin, INPUT);
  pinMode(resetPin, OUTPUT);
  digitalWrite(resetPin, HIGH);
  delay(fumeHoodMonitorBootTime);
  watchDogTimer = millis();
}

void loop(){
  watchDogIsBeingPet = digitalRead(watchDogPin) == 1;
  if (watchDogIsBeingPet) {
    watchDogHasBeenPet = true;
  } else if (!watchDogIsBeingPet && watchDogHasBeenPet) {
    watchDogTimer = millis();
    watchDogHasBeenPet = false;
  } 
  unsigned long now = millis();
  Serial.println(now - watchDogTimer);
  if(now - watchDogTimer >= watchDogInterval){
    digitalWrite(resetPin, LOW);
    delay(resetDuration);
    digitalWrite(resetPin, HIGH);
    delay(fumeHoodMonitorBootTime);
    watchDogTimer = millis();
  }
}
