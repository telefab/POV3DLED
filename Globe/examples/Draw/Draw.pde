/**
 * Draw on the globe from the bus
 */

#include <Globe.h>
#include <BusSlave.h>
#include <Pin.h>

#define MODE_NONE 0
#define MODE_ALL 1
#define MODE_DIFF 2
#define STEP_MODE 0
#define STEP_ROTATION 1
#define STEP_X 2
#define STEP_Y 3
#define STEP_COLOR 4

Globe *globe = Globe::get();
BusSlave *bus = BusSlave::get();

uint8_t mode = MODE_NONE;

uint8_t curX = 0;
uint8_t curY = 0;

uint8_t step = STEP_ROTATION; 

void setup() {
  globe->begin();
  bus->begin();
}

void loop() {
  if (bus->available()) {
    uint8_t value = bus->read();
    if (value == 255) {
      // Reset the mode
      step = STEP_MODE;
      curX = 0;
      curY = 0;
    } else if (step == STEP_MODE) {
      // Start a new transaction
      step = STEP_ROTATION;
      if (value == 0)
        mode = MODE_ALL;
      else
        mode = MODE_DIFF;
    } else if (step == STEP_ROTATION) {
      // Receive the current rotation status
      globe->clearRotation();
      globe->rotate(value);
      step = STEP_X;
    } else if (mode == MODE_ALL) {
      // Receive all LEDs colors
      if (curY < globe->getHeight()) {
        globe->setLed(curX, curY, value);
        curX++;
        if (curX >= globe->getWidth()) {
          curX = 0;
          curY++;
        }
      }
    } else {
      // Receive some LED colors
      if (step == STEP_X) {
        curX = value;
        step = STEP_Y;
      } else if (step == STEP_Y) {
        curY = value;
        step = STEP_COLOR;
      } else {
        globe->setLed(curX, curY, value);
        step = STEP_X;
      }
    }
  }
}
