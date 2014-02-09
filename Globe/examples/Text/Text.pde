/**
 * Display some text rotating on the globe.
 */

#define TEXT_COLOR 1

#include <Globe.h>
#include <Pin.h>

const char text[] = "Bonjour";

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
const unsigned int globeHeight = globe->getHeight();

uint8_t i, currentPos, currentIndex, currentCol, endFrom;

void setup() {
  currentPos = textLength;
  currentCol = 0;
  endFrom = 0;
  globe->begin();
}

void loop() {
  // Always keep the 8 last columns clear to separate start and end of text
  for (i = 0; i < globeHeight; i++) {
    globe->setLedRot(globeWidth-8, i, 0);
  }
  // If the text is not fully displayed
  if (endFrom == 0) {
    // Select the right character
    if (currentCol == 0) {
      if (currentPos > 0) {
        currentPos--;
      } else {
        currentPos = textLength-1;
        endFrom = 1;
      }
      currentCol = 7;
      // Select the index in the letter array
      currentIndex = text[currentPos];
      if (currentIndex >= 65 && currentPos <= 90)
        currentIndex-= 64;
      else if (currentIndex >= 97 && currentIndex <= 122)
        currentIndex-= 96;
      else
        currentIndex = 0;
    } else {
      currentCol--;
    }
    // Display one column of text
    if (endFrom == 0) {
      for (i = 0; i < globeHeight; i++) {
        globe->setLedRot(0, i, ((letters[currentIndex][currentCol] >> i) && 1) * TEXT_COLOR);
      }
    }
  } else {
    // Wait until the text has made a full round before displaying again
    if (endFrom >= globeWidth - 8)
      endFrom = 0;
    else
      endFrom++;
  }
  // Rotate
  globe->rotate(1);
  globe->delayRound(1);
}
