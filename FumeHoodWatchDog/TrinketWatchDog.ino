#define watchDogPin 0
#define resetPin 2
#define heartBeatPin 1

unsigned long watchDogInterval = 7000;
unsigned long resetDuration = 5000;
unsigned long watchDogTimer;

unsigned long heartBeatTimer;
unsigned long heartBeatDuration = 15;
short heartBeatBrightness = 0;
short fadeAmount = 5;

bool watchDogHasBeenPet = FALSE;

void setup() {
  pinMode(watchDogPin, INPUT);
  pinMode(resetPin, OUTPUT_PULLUP);
  pinMode(heartBeatPin, OUTPUT);
  watchDogTimer = millis();
  heartBeatTimer = millis();
}

void loop(){
  watchDogHasBeenPet = digitalRead(watchDogPin) == 1;
  if (watchDogHasBeenPet) watchDogTimer = millis();
  unsigned long now = millis();
  if(now - watchDogTimer >= watchDogInterval){
    digitalWrite(resetPin, LOW);
    digitalWrite(heartBeatPin, HIGH);
    delay(resetDuration);
    watchdogTimer = millis();
  }
  heartBeat();
}

void heartBeat(){
  unsigned long now = millis();
  if (now - heartBeatTimer >= heartBeatDuration){
    analogWrite(heartBeatPin, brightness);
    brightness += fadeAmount;
    if (brightness <= 0 || brightness >= 255) fadeAmount = -fadeAmount;
  }
}
