/**
 * Snake game, one or two players.
 * In two players mode, the globe is divided into two zones.
 * Each player controls the snake only when it is in the right zone.
 */

#include <Globe.h>
#include <BusSlave.h>
#include <Pin.h>

// Game colors (RGB bitmask)
#define SNAKE_COLOR 4
#define SNAKE_HEAD_COLOR(zone) ((zone) == 1 ? 5 : 6)
#define TARGET_COLOR 2

// Size increase at each found target
#define GROWTH_RATE 3

// Possible directions
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

Globe *globe = Globe::get();
BusSlave *bus = BusSlave::get();

const uint8_t globeWidth = globe->getWidth();
const uint8_t globeHeight = globe->getHeight();
const uint8_t globeZone = globeHeight/2;

// Snake head location and direction
uint8_t headX;
uint8_t headY;
uint8_t headZone;
uint8_t headDir;
// Snake tail location
uint8_t tailX;
uint8_t tailY;
// Snake size
uint16_t size;
uint16_t visibleSize;
// Target current zone
uint8_t targetZone;
// Last received order data
uint8_t receivedOrder;
uint8_t receivedDir;
uint8_t receivedZone;
// Temporary variables
uint8_t tmpX;
uint8_t tmpY;
uint8_t tmpDir;
uint8_t tmpColor;

/**
 * Get the next position and direction depending on the current position
 * and direction
 */
void nextPos(uint8_t &nextX, uint8_t &nextY, uint8_t &nextDir, const uint8_t& x, const uint8_t& y, const uint8_t& dir) {
  nextX = x;
  nextY = y;
  nextDir = dir;
  switch(dir) {
    case UP:
      if (nextY == 0) {
        // Crossing the top
        nextDir = DOWN;
        if (nextX < headZone)
          nextX+= headZone;
        else
          nextX-= headZone;
      } else {
        nextY--;
      }
      break;
    case DOWN:
      if (nextY == globeHeight-1) {
        // Crossing the bottom
        nextDir = UP;
        if (nextX < headZone)
          nextX+= headZone;
        else
          nextX-= headZone;
      } else {
        nextY++;
      }
      break;
    case LEFT:
      if (nextX == 0) {
        nextX = globeWidth-1;
      } else {
        nextX--;
      }
      break;
    case RIGHT:
      if (nextX == globeWidth-1) {
        nextX = 0;
      } else {
        nextX++;
      }
      break;
  }
}

/**
 * Create a new random target
 * alternating the zone
 */
void createTarget() {
  targetZone = targetZone == 1 ? 2 : 1;
  do {
    tmpY = random(globeHeight);
    // Create the target in the right zone
    if (targetZone == 1)
      tmpX = random(globeZone);
    else
      tmpX = random(globeZone, globeWidth);
  } while (globe->getLed(tmpX, tmpY) != 0);
  // Draw the target
  globe->setLed(tmpX, tmpY, TARGET_COLOR);
}

/**
 * Initialize the game variables
 */
void initGame() {
  // Make the display all black
  for (tmpX = 0; tmpX < globeWidth; tmpX++)
    for (tmpY = 0; tmpY < globeHeight; tmpY++)
      globe->setLed(tmpX, tmpY, 0);
  // Initial position with size 1
  headX = globeZone/2;
  headY = globeHeight/2;
  headZone = 1;
  headDir = LEFT;
  tailX = headX;
  tailY = headY;
  size = 1;
  visibleSize = 1;
  // Target will start in zone 2
  targetZone = 1;
  // Initialize the display
  snakeStep();
  createTarget();
}

/**
 * Make the snake move by one step
 */
void snakeStep() {
  // Future position of the head
  nextPos(tmpX, tmpY, tmpDir, headX, headY, headDir);
  tmpColor = globe->getLed(tmpX, tmpY) & 0x07;
  if (tmpColor == TARGET_COLOR) {
    // Reached a target, grow
    size+= GROWTH_RATE;
    createTarget();
  } else if (tmpColor == SNAKE_COLOR && (visibleSize < size || tmpX != tailX || tmpX != tailY)) {
    // The snake did byte itself (tail ignored if the snake is not getting bigger)
    initGame();
    return;
  }
  // Move the head
  // Set the previous head as body part
  // Include the direction in unused bits to be able to locate the next snake part
  globe->setLed(headX, headY, SNAKE_COLOR | (headDir << 3));
  // Set the new head
  headX = tmpX;
  headY = tmpY;
  headDir = tmpDir;
  headZone = headX < globeZone ? 1 : 2;
  globe->setLed(headX, headY, SNAKE_HEAD_COLOR(headZone));
  // Move the tail if the snake is not growing
  if (size == visibleSize) {
    tmpDir = globe->getLed(tailX, tailY) >> 3;
    globe->setLed(tailX, tailY, 0);
    nextPos(tailX, tailY, tmpDir, tailX, tailY, tmpDir);
  } else {
    // The snake has grown of one element
    visibleSize++;
  }
}

/**
 * Process an order received through the bus
 */
void processOrder() {
  // Read the bus byte
  receivedOrder = bus->read();
  receivedDir = receivedOrder & 0x03;
  receivedZone = (receivedOrder >> 2) & 0x03;
  // Check the zone (player 1 or 2) from which the order is coming
  // Zone 0 is for one player mode
  if (receivedZone == 0 || receivedZone == headZone) {
    // Change the current direction
    headDir = receivedDir;
  }
}

/**
 * Arduino setup phase
 */
void setup() {
  randomSeed(analogRead(0));
  globe->begin();
  bus->begin();
  initGame();
}

/**
 * Arduino ever-running loop
 */
void loop() {
  if (globe->passedRound()) {
    // New round, time for the snake to make one step
    snakeStep();
  }
  // Listen to the bus for game orders
  if (bus->available()) {
    // new order received, process
    processOrder();
  }
}