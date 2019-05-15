#include <Adafruit_CircuitPlayground.h>
#include <Wire.h>
#include <SPI.h>



#define signalToRelayPin 13
#define sensorPin A4
#define LEDSOTE 17
#define sensorlight A5

int lastSoundValue;
int soundValue;
long lastNoiseTime = 0;
long currentNoiseTime = 0;
long lastLightChange = 0;
int relayStatus = HIGH;

int soundValuesote;
int ledStatusote = HIGH;

void setup() {
  CircuitPlayground.begin();
  pinMode(sensorPin, INPUT);
  pinMode(signalToRelayPin, OUTPUT);

  pinMode(sensorlight, INPUT);
  pinMode(LEDSOTE, OUTPUT);
}

void loop() {

  soundValuesote = digitalRead(sensorlight);
  if (soundValuesote == 1) 
  CircuitPlayground.setPixelColor(0, 255, 0, 0);
  
  CircuitPlayground.clearPixels();
  
  soundValue = digitalRead(sensorPin);
  currentNoiseTime = millis();

  if (soundValue == 1) { // if there is currently a noise

    if (
      (currentNoiseTime > lastNoiseTime) && // to debounce a sound occurring in more than a loop cycle as a single noise
      (lastSoundValue == 0) &&  // if it was silent before
      (currentNoiseTime < lastNoiseTime + 500) && // if current clap is less than 0.8 seconds after the first clap
      (currentNoiseTime > lastLightChange + 500) // to avoid taking a third clap as part of a pattern
    ) {

      relayStatus = !relayStatus;
      digitalWrite(signalToRelayPin, relayStatus);
      lastLightChange = currentNoiseTime;
     }

     lastNoiseTime = currentNoiseTime;
  }

  lastSoundValue = soundValue;
}
