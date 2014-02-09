/**
 * Runner trying to run faster than the globe
 */

#include <Globe.h>
#include <Pin.h>

// Configuration
#define COLOR 4 // Color of the runner
#define STEP_SIZE 2 // Speed when running (columns per frame)
#define FRAME_DELAY 1 // Animation speed (in number of rounds)
#define STATIC_FRAMES 50 // Frames during which the runner stays at 0

// Images of a runner (each bit is an image, 8 images are mixed)
#define RUNNER_WIDTH 24
#define RUNNER_HEIGHT 31
#define RUNNER_FRAMES 8

const uint8_t runners[RUNNER_HEIGHT*RUNNER_WIDTH] = {
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 72, 76,136,  8,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 76,204,204,221,157,  8,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 68,239,255,255,255,255,189,153,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 68,239,255,255,255,255,189,153,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 70,239,255,255,255,255,191,185,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 70,239,255,255,255,255,191,185,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 66,239,255,255,255,255,191,177,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 66,238,239,255,255,255,187, 49,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0, 72,206,238,239,255,255,191,179, 49,  0,  8,  8,  8,  0,  0,
  0,  0,  0,  0,  0,  0,  8,200,200,232,238,255,255,255,255,255,152,136,  8,137,137,137,136,  0,
  0,  0,  0,  0,  0,  0,232,248,248,255,255,255,255,255,255,221,137,152,153,153,153,137,137,  0,
  0,  0,  0,  0,128,128,249,251,123,127,255,255,255,255,255,157,153,153,153,153,153,137,129,  0,
  0,  0,  0,  0,128,128,249,251,251,255,127,255,255,255,255,157,217,217,217,153,153,137,129,  0,
  0,  0,  0,  0,128,129,251,250,251,255,255,255,255,251,255,221,217,217,217,217,153,129,129,  0,
  0,  0,  0,  0,  1,225,243,211,243,243,255,127,191,255,221,253,255,127, 81, 65,  1,  0,  0,  0,
  0,  0,  0,  0,  1, 65,243,211,243,243,247,222,223,159,255,255,255,127, 99, 64,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  1, 99, 67, 99,115,247,248,253,191,255,255,255,102, 34, 32,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0, 34,  2,115,246,255,191,251,255,239,111,238, 44, 12, 12,  0,  0,  0,
  0,  0,136,136,128,128,  0,  0,  0, 50,247,255,255,255,255,255,236,238,204,204,140, 12,  8,  0,
  0,  0,140,200,200,192,128,128,  0,144,255,255,255,255,255,255,237,236,204,204,204,140,  8,  0,
  0,  4,204,204,200,200,192,128,128,254,253,255,255,255,255,223,205,204,204,204,204,140,  8,  0,
  4, 12,204,204,220,220,220,221,221,254,223,223,255,255,255,247,229,236,204,204,204,136,  8,  0,
  4, 12,204,204,220,220,221,221,255,255,239,111,255,255,119,119,229,229,204,204,204,136,  8,  0,
  4, 12,140,204,220,220,255,255,255,255,255,255,252, 51,119,119,101,229,200,136,136,136,  0,  0,
  4, 12, 12,204,220,212,127,127,127,255,255, 52, 51, 51,119,119,229,228,204,200,136,136,  0,  0,
  0,  4, 12, 12, 80, 80, 35, 35, 51, 51, 50, 17, 17, 17, 81,193,196,196,204,204,200,136,  8,  8,
  0,  0,  4,  4, 16, 16, 35, 35, 35, 18, 19, 49, 49, 49,  1,193,192,192,200,200,136,136,  8,  0,
  0,  0,  0,  0, 16, 16, 35, 35,  3, 34, 33, 49, 49, 17,  1,  0,128,128,128,128,128,128,  0,  0,
  0,  0,  0,  0,  2,  2,  2, 34, 34, 50, 49, 49, 17, 17,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  2,  2, 34, 34, 34, 51, 49, 49, 49, 17,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  2,  2,  2, 34, 34, 51, 49, 49, 49, 49, 17,  0,  0,  0,  0,  0,  0,  0,  0,  0
};


uint8_t currentFrame = 0;
uint16_t staticDelay = 0;

Globe *globe = Globe::get();

void setup() {
  globe->begin();
}

void loop() {
  // Refresh the display
  int i, j, left, top;
  uint8_t color;
  for (i = 0; i < RUNNER_WIDTH; i++) {
    for (j = 0; j < RUNNER_HEIGHT; j++) {
      // Find the position of the LED on the globe
      left = RUNNER_WIDTH - 1 - i;
      top = j + (globe->getHeight() - RUNNER_HEIGHT)/2;
      // Decide the color
      if ((runners[i + j * RUNNER_WIDTH] & (1 << currentFrame)) == 0)
        color = 0;
      else
        color = COLOR;
      // Set the color
      globe->setLed(left, top, color);
    }
  }
  // Let the runner move sometimes
  if (staticDelay == STATIC_FRAMES) {
    globe->rotate(-STEP_SIZE);
    if (globe->getRotation() < STEP_SIZE) {
      globe->clearRotation();
      // Stop moving
      staticDelay = 0;
    }
  } else {
    // Count until next move
    staticDelay++;
  }
  // Prepare for next frame
  currentFrame++;
  if (currentFrame >= RUNNER_FRAMES)
    currentFrame = 0;
  globe->delayRound(FRAME_DELAY);
}