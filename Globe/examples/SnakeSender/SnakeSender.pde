/**
 * Snake controller.
 * Works with two joysticks.
 * Simply send orders through the bus.
 */

#include <Pin.h>
#include <BusMaster.h>

// Detection margin (range 1024/2)
#define MARGIN 100

// Possible directions
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define NO_MOVE 4

BusMaster *bus = BusMaster::get();

/**
 * Sensors used for each player
 */
const uint8_t sensors[2][2] = {
  {A2, A3},
  {A4, A5}
};

/**
 * Current direction
 */
uint8_t dir[] = {NO_MOVE, NO_MOVE};

/**
 * Temporary values
 */
uint16_t tmpValue;
uint8_t tmpOrder;
uint8_t tmpDir;
uint8_t tmpData;

/**
 * Analyze a sensor value.
 * Return 0 if no detection, 1 or 2 depending on the detection
 */
uint8_t analyzeSensor(const uint8_t& sensor) {
  tmpValue = analogRead(sensor);
  if (tmpValue >= (1024 - MARGIN))
    return 2;
  if (tmpValue <= MARGIN)
    return 1;
  return 0;
}

/**
 * Check direction for one player
 */
void checkDir(uint8_t player) {
  // Read the UP/DOWN sensor
  tmpOrder = analyzeSensor(sensors[player][0]);
  tmpDir = NO_MOVE;
  if (tmpOrder == 2)
    tmpDir = UP;
  else if (tmpOrder == 1)
    tmpDir = DOWN;
  // Read the LEFT/RIGHT sensor
  tmpOrder = analyzeSensor(sensors[player][1]);
  if (tmpOrder == 2)
    tmpDir = LEFT;
  else if (tmpOrder == 1)
    tmpDir = RIGHT;
  if (tmpDir != NO_MOVE && tmpDir != dir[player]) {
    // New direction
    tmpData = tmpDir;
    // Include the player number in sent data
    tmpData|= (player + 1) << 2;
    // Send
    bus->send((char*) &tmpData, 1);
  }
  // Save the direction
  dir[player] = tmpDir;
}

void setup() {
  bus->begin();
}

void loop() {
  checkDir(0);
  checkDir(1);
}