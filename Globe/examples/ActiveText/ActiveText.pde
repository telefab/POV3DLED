/**
 * Display some text rotating on the globe.
 * There is a text by default, and it can be changed by sending text through the bus.
 * To change the color, send #3, where 3 is replaced by the number of the wanted color.
 */

#define CHAR_WIDTH 5
#define CHAR_SPACE 1
#define CHAR_FULL_WIDTH (CHAR_WIDTH+CHAR_SPACE)
#define CHAR_HEIGHT 5

/**
 * Synchronize these settings between master and slave
 */
#define MAX_LENGTH 25

#include <Globe.h>
#include <BusSlave.h>
#include <Pin.h>

/**
 * Change here the text to display
 */
char text[MAX_LENGTH] = "Telecom Bretagne";
/**
 * Change here the color of the text
 * It is a bit mask with 3 bits Red, Greed, and Blue
 */
uint8_t textColor = 1;

/**
 * Text received through the bus
 */
char receivedText[MAX_LENGTH];

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
BusSlave *bus = BusSlave::get();

unsigned int textLength = strlen(text);
const unsigned int globeWidth = globe->getWidth();
const unsigned int heightOffset = (globe->getHeight() - CHAR_HEIGHT)/2;

uint8_t i, currentPos, currentIndex, currentCol, endFrom;

void setup() {
  currentPos = 0;
  currentCol = 0;
  endFrom = 0;
  globe->begin();
  bus->begin();
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
      globe->setLedRot(globeWidth-CHAR_FULL_WIDTH, i+heightOffset, ((letters[currentIndex][i] >> (CHAR_FULL_WIDTH-currentCol)) & 1) * textColor);
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
  delay(100);
  // Bus communication (change text and color)
  for (uint16_t i = 0; i < MAX_LENGTH; i++) {
    if (bus->readString(receivedText, MAX_LENGTH)) {
      if (receivedText[0] == '#' && strlen(receivedText) == 2) {
        // Change the text color
        textColor = receivedText[1] - 48;
      } else {
        // Change the text itself
        strcpy(text, receivedText);
        textLength = strlen(text);
        currentPos = 0;
        currentCol = 0;
        endFrom = 0;
        globe->rotate(-5);
      }
    }
  }
}