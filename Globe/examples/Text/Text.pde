/**
 * Display some text rotating on the globe.
 */

#define TEXT_COLOR 1 // Color: 1 red, 2 green, 4 blue
#define CHAR_WIDTH 5
#define CHAR_SPACE 1
#define CHAR_FULL_WIDTH (CHAR_WIDTH+CHAR_SPACE)
#define CHAR_HEIGHT 5

#include <Globe.h>
#include <Pin.h>

/**
 * Change here the text to display
 */
const char text[] = "Telecom Bretagne";

/**
 * Space, then letters from A to Z.
 * Each letter is 5x5 pixels
 */
const byte letters[27][5] = {
  {0x00,0x00,0x00,0x00,0x00},
  {0x7c,0x44,0x44,0x7c,0x44},    
  {0x7c,0x44,0x78,0x44,0x7c},  
  {0x7c,0x40,0x40,0x40,0x7c},  
  {0x78,0x44,0x44,0x44,0x78},  
  {0x7c,0x40,0x78,0x40,0x7c},
  {0x7c,0x40,0x70,0x40,0x40},
  {0x7c,0x40,0x4c,0x44,0x7c},
  {0x44,0x44,0x7c,0x44,0x44},
  {0x7c,0x10,0x10,0x10,0x7c},
  {0x0c,0x04,0x04,0x44,0x7c},
  {0x44,0x48,0x70,0x48,0x44},
  {0x40,0x40,0x40,0x40,0x7c},
  {0x44,0x6c,0x54,0x44,0x44},
  {0x44,0x64,0x54,0x4c,0x44},
  {0x38,0x44,0x44,0x44,0x38},  
  {0x78,0x44,0x78,0x40,0x40},              
  {0x7c,0x44,0x44,0x7c,0x10},            
  {0x78,0x44,0x78,0x44,0x44},            
  {0x7c,0x40,0x7c,0x04,0x7c},            
  {0x7c,0x10,0x10,0x10,0x10},              
  {0x44,0x44,0x44,0x44,0x7c},            
  {0x44,0x44,0x28,0x28,0x10},            
  {0x44,0x44,0x54,0x54,0x28},            
  {0x44,0x28,0x10,0x28,0x44},            
  {0x44,0x44,0x28,0x10,0x10},            
  {0x7c,0x08,0x10,0x20,0x7c}
};

Globe *globe = Globe::get();

const unsigned int textLength = strlen(text);
const unsigned int globeWidth = globe->getWidth();
const unsigned int heightOffset = (globe->getHeight() - CHAR_HEIGHT)/2;

uint8_t i, currentPos, currentIndex, currentCol, endFrom;

void setup() {
  currentPos = 0;
  currentCol = 0;
  endFrom = 0;
  globe->begin();
}

void loop() {
  // Always keep the last columns clear to separate start and end of text
  for (i = 0; i < CHAR_HEIGHT; i++) {
    globe->setLedRot(0, i+heightOffset, 0);
  }
  // If the text is not fully displayed
  if (endFrom == 0) {
    if (currentCol == 0) {
      // Select the index of the character in the letter array
      currentIndex = text[currentPos];
      if (currentIndex >= 65 && currentIndex <= 90)
        currentIndex-= 64;
      else if (currentIndex >= 97 && currentIndex <= 122)
        currentIndex-= 96;
      else
        currentIndex = 0;
    }
    // Display one column of text
    for (i = 0; i < CHAR_HEIGHT; i++) {
      globe->setLedRot(globeWidth-CHAR_FULL_WIDTH, i+heightOffset, ((letters[currentIndex][i] >> (CHAR_FULL_WIDTH-currentCol)) & 1) * TEXT_COLOR);
    }
    // Get ready for next column
    currentCol++;
    if (currentCol > CHAR_WIDTH) {
      currentCol = 0;
      currentPos++;
      if (currentPos > (textLength-1)) {
        currentPos = 0;
        endFrom = 1;
      }
    }
  } else {
    // Wait until the text has made a full round before displaying again
    if (endFrom >= globeWidth - CHAR_FULL_WIDTH)
      endFrom = 0;
    else
      endFrom++;
  }
  // Rotate
  globe->rotate(-1);
  globe->delayRound(1);
}
