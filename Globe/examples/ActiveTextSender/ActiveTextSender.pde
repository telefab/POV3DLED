/**
 * Receive text from Serial and send it through the bus to the globe.
 * This should not be uploaded on the Arduino of the globe.
 */

#include <Pin.h>
#include <BusMaster.h>

/**
 * Synchronize these settings between master and slave
 */
#define MAX_LENGTH 25

BusMaster *bus = BusMaster::get();
char message[MAX_LENGTH];
char inByte = 0;
uint16_t messageIndex = 0;

void setup() {
  Serial.begin(9600);
  bus->begin();
}

void loop() {
  if (Serial.available() > 0) {
    inByte = Serial.read();
    if (inByte == '\n' || messageIndex == (MAX_LENGTH-1)) {
      message[messageIndex] = '\0';
      bus->send(message);
      messageIndex = 0;
    } else {
      message[messageIndex] = inByte;
      messageIndex++;
    }
  }
}