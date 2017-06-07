// This is a watchdog program to be run on an Adafruit Trinket

#define watchDogPin 1
#define resetPin 2
#define resetButtonPin 4

static const int resetHoldTime = 5000; //ms
static const int fumeHoodMonitorBootTime = 20000; //ms
static const int noChangeCountResetTrigger = 300; // equals 30 seconds with a system check every 100 ms
static const int resetButtonCountResetTrigger = 20; // equals 2 seconds with a system check every 100 ms
static const int systemCheckInterval = 100; // ms between system checks
int noChangeCount = 0;
int resetButtonCount = 0;
int watchDogInputState = 0;
int lastWatchDogInputState = 0;

void setup() {
  pinMode(watchDogPin, INPUT);
  pinMode(resetPin, OUTPUT);
  pinMode(resetButtonPin, INPUT);
  digitalWrite(resetPin, HIGH);
  delay(fumeHoodMonitorBootTime);
}

void loop(){
  checkSystem(); // continuously monitors input pulse and will only break if the pulse stops
  resetSystem(); // will only execute if the watchdog is not changing state
}

void checkSystem(){
  do {
    // Monitor watchDog pulses
    watchDogInputState = digitalRead(watchDogPin);
    if (watchDogInputState == lastWatchDogInputState) noChangeCount++;
      else noChangeCount = 0;
    lastWatchDogInputState = watchDogInputState;

    // Monitor Reset Button
    if (digitalRead(resetButtonPin) == HIGH) resetButtonCount++;
      else resetButtonCount = 0;

    delay(systemCheckInterval);
//  } while(true);
   } while(noChangeCount < noChangeCountResetTrigger && resetButtonCount < resetButtonCountResetTrigger);
}

void resetSystem() {
  digitalWrite(resetPin, LOW);
  delay(resetHoldTime);
  digitalWrite(resetPin, HIGH);
  delay(fumeHoodMonitorBootTime);
  noChangeCount = 0;
  resetButtonCount = 0;
}
