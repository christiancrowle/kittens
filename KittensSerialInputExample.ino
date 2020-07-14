// this is an example arduino sketch for serial input in kittens.
// Kittens wants a big ol' list of pin states as a newline separated string where each character is a '1' for connected and '0' for disconnected.
// the actual connected/disconnected state doesn't really matter much, the serial input just watches for any changing pins and considers it a pulse.
//    (hey look, now your toggle switch is a pushbutton!)
// probably there's a better way of doing it. This is what I use for my band.
// also this file is public domain or whatever. Tested on an arduino uno r3 clone (elegoo r3). will likely work on other AVR based inos.

// PS. note to future me, because you absolutely will fuck this up: this needs an older, pre 1.8.3 version of "arduino avr boards". updating will fuck shit up.
#include <ArduinoSTL.h>

void setup() {
  Serial.begin(115200); // 115200 baud because we like fast or something
  for (int pin = 0; pin < 7; pin++) { // every pin should start as pullup'd input
    pinMode(pin + 2, INPUT_PULLUP);
  }
}

std::vector<int> old_connections;
void loop() {
  std::vector<int> connections;

  int numConnections = 0;

  // detect continuity. for each pin on the arduino (at least each one we care about),
  // check if they're connected by doing the following steps:
  //      1. set pin to be an output
  //      2. drive it LOW
  //      3. now check every other pin:
  //          3a. is it low?
  //          3b. if yes, it must be connected. add it to our vector of pins.
  //      4. set the pin back to a pullup'd input
  for (int pin = 2; pin < 11; pin++) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);

    for (int pp = pin + 1; pp < 11; pp++) {
      if (digitalRead(pp) == LOW) {
        connections.push_back(1);
      } else {
        connections.push_back(0);
      }
    }
    
    pinMode(pin, INPUT_PULLUP);
  }

  // did anything actually change?
  if (connections != old_connections) {
    for (int i = 0; i < connections.size(); i++) { // print all of the pin states as a big ol' number
      Serial.print(connections[i]);
    }
    Serial.println();
  }

  old_connections = connections; // now make sure we keep track of the current state so we can compare it next time.
}
