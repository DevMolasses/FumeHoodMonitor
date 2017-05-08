// This is a watchdog program to be run on an Adafruit Trinket

#define watchDogPin 1
#define resetPin 2

unsigned long watchDogInterval = 7000;
unsigned long resetDuration = 5000;
unsigned long watchDogTimer;

unsigned long heartBeatTimer;
unsigned long heartBeatDuration = 15;
short heartBeatBrightness = 0;
short fadeAmount = 5;

bool watchDogHasBeenPet = false;

void setup() {
  pinMode(watchDogPin, INPUT);
  pinMode(resetPin, OUTPUT);
  digitalWrite(resetPin, HIGH);
  watchDogTimer = millis();
  heartBeatTimer = millis();
}

void loop(){
  watchDogHasBeenPet = digitalRead(watchDogPin) == 1;
  if (watchDogHasBeenPet) watchDogTimer = millis();
  unsigned long now = millis();
  if(now - watchDogTimer >= watchDogInterval){
    digitalWrite(resetPin, LOW);
    delay(resetDuration);
    digitalWrite(resetPin, HIGH);
    delay(15000); // wait 15 seconds for monitor to reboot
    watchDogTimer = millis();
  }
}
