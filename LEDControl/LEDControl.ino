/**
 * LED controller
 * Displays a fixed image on the sphere.
 * synchronization coded but disabled for now: fixed rounds per second.
 */

// Number of LEDs in the column
#define LEDS 39

// Number of colors per LED
#define COLORS 3

// Number of virtual columns in one round
#define COLUMNS 77

// Positive LED pins
const byte ledPins[] = {A2,A3,A4,A5,A6,A7,A8,A9,A10,52,53,50,51,48,49,46,47,44,45,42,43,40,41,38,39,36,37,34,35,32,33,30,31,28,29,26,27,24,25};

// Negative color LED pins (red, green, blue)
const byte colorPins[] = {22,A0,23};

// Image data LED by LED
// Colors: bit mask
// red => 1
// green => 2
// blue => 4
const byte imageData[COLUMNS][LEDS] = {
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,1,1,1,1,1,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,1,0,0,0,1,1,1,0,0,1,1,1,1,1,1,1,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,1,1,0,0,1,0,1,0,0,0,1,1,1,1,1,1,1,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {1,1,1,1,0,1,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {1,1,1,0,0,1,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,2,2,2,2,0,0,0,0,0,0,0,0,0},
    {1,1,0,0,0,0,1,3,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,0,0,0,0,0,2,2,2},
    {1,1,0,0,0,0,1,1,1,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
    {0,0,1,1,0,0,0,1,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0},
    {0,1,1,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,0,0},
    {0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2,2,0,0,0},
    {0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,0,0,0,0,0},
    {1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,2,0,0,0,0,0,0},
    {1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,0,0,0,0,0,0,0},
    {1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0},
    {1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,3,3,3,3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,3,3,3,3,3,3,3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,3,0,3,3,3,3,3,3,3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,3,3,3,0,0,3,3,3,3,3,3,3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,3,0,0,3,3,3,0,0,3,3,3,3,3,3,3,3,3,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,3,3,3,3,0,3,3,3,0,0,0,3,3,3,3,3,3,3,3,3,3,3,3,3,3,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,3,3,3,0,0,0,3,3,3,0,0,0,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,0,0,0,0},
    {0,0,0,0,0,0,3,3,0,0,0,3,3,3,3,3,0,0,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,0,0,0},
    {0,0,0,0,0,0,3,3,3,3,3,3,3,3,3,0,0,0,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,0,0,0,0},
    {0,0,0,0,0,0,3,3,3,3,3,3,3,3,0,4,4,0,0,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,3,3,3,3,3,3,0,0,4,4,4,4,0,3,3,3,3,3,3,3,3,3,3,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,0,3,3,3,3,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,0,3,3,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,4,4,4,4,4,4,4,0,0,0,4,4,0,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,4,4,4,4,4,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,4,5,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,4,4,4,4,4,4,4,4,4,4,4,0,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,5,5,5,0,0,0,0},
    {0,0,0,0,0,4,4,4,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,5,5,5,0,0,0,0},
    {0,0,0,0,0,4,4,4,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,5,5,5,0,0,0,0},
    {0,0,0,0,0,0,4,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,5,5,5,5,0,0,0,0},
    {0,0,0,0,0,0,4,4,4,4,4,0,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,5,5,0,0,0,0},
    {0,0,0,0,0,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,5,5,5,5,0,0,0},
    {0,0,0,0,0,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,5,5,5,0,0,0},
    {0,0,0,0,0,0,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,4,4,4,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,4,4,4,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};

// Dynamic image data for non-static images
byte dynamicData[COLUMNS][LEDS];

// Delay to display one color of one column (microseconds)
unsigned long ledDelay = 50;

// Currently displayed colum  n
int currentColumn = COLUMNS-1;

// Currently displayed shade
int currentLed = LEDS-1;

// Date (microseconds) of the last LED refresh
unsigned long lastRefresh = 0;

// Date (microseconds) of the last round start (photoresistor)
unsigned long lastRoundStart = 0;

// Date (microseconds) of the current r
unsigned long curStart = 0;

// Is the display paused due to too slow cycles?
bool paused = false;
int lastState = LOW;

// Display given data column on the LEDs, for the given color
// Warning: to protect LEDS, they should never be on too long!
// The voltage is higher than the static limit!
void displayColumnLed(const byte data[COLUMNS][LEDS], int column, int led) {
    int i;
    // Switch the previous led off
    digitalWrite(ledPins[currentLed], LOW);
    // Select the right colors
    for (i = 0; i < COLORS; i++) {
        digitalWrite(colorPins[i], (data[column][led] & (1 << i)) == 0 ? HIGH : LOW);
    }
    // Switch the proper led on
    digitalWrite(ledPins[led], HIGH);
    // Remember current display
    currentLed = led;
    currentColumn = column;
}

// Refresh the column display based on the current state and on data
// Only if the delay since the last refresh is enough
boolean refreshImage(const byte data[COLUMNS][LEDS]) {
    unsigned long curDelay = micros() - lastRefresh;
    // The 3 is to compensate the inaccuracy (mean is ok)
    if (curDelay >= ledDelay) {
        // Start a new delay
        lastRefresh = micros();
        paused = false;
        // Cycle through colors and columns
        // Repeating the color cycle for each column
        int newLed = currentLed + 1;
        int newColumn = currentColumn; 
        if (newLed > LEDS)
        {
            newLed = 0;
            newColumn++;
            if (newColumn >= COLUMNS)
                newColumn = 0;
        }
        // Display
        displayColumnLed(data, newColumn, newLed);
        return true;
    }
    return false;
}

// Simple blocking version of refreshImage
void iterateImage(const byte data[COLUMNS][LEDS]) {
    while (!refreshImage(data));
}

// Full display with an unique shade during 1 second
void displayShade(byte shade) {
    unsigned long endTime = millis() + 1000;
    // Data with all LEDs on
    int i, j;
    for (i = 0; i < COLUMNS; i++)
        for (j = 0; j < LEDS; j++)
            dynamicData[i][j] = shade;
    // Blocking display
    while (millis() < endTime)
        iterateImage(dynamicData);
}

// Initialization method
void setup() {
    // Serial debug initialization
    //Serial.begin(9600);
    // LED pins initialization
    int i;
    for (i = 0; i < LEDS; i++) {
        pinMode(ledPins[i], OUTPUT);
    }
    for (i = 0; i < COLORS; i++) {
        pinMode(colorPins[i], OUTPUT);
    }
    // Set the LED sensor interrupt on pin 2
    // Internal pull-up resistor used instead of external
    pinMode(2, INPUT_PULLUP);
    //attachInterrupt(0, startRound, FALLING);
    // Test sequence (to check LEDs)
    delay(100);
    /*Serial.print("LED delay: ");
    Serial.println(ledDelay);
    Serial.println("Test sequence");*/
    for (i = 0; i < COLORS; i++) {
      /*  Serial.print("Color /");
        Serial.println(i);*/
        displayShade(1 << i);
    }
    //Serial.println("End test sequence");
}

boolean isNewRound()
{
    int currentState = digitalRead(2);
    boolean newRound = (currentState == HIGH && lastState == LOW) ? true : false;
    lastState = currentState;
    return newRound;
   
}

// Main method looping forever
void loop() {
    // Check regularly if a LED was on too long
   // delay(5);
     // Log the round start and delay
  if(isNewRound())
  {
    startRound();
  }
  /*  unsigned long curDelay = micros() - lastRefresh;
    // Pause the display if necessary
    if (!paused && curDelay >= 1000) {
        digitalWrite(ledPins[currentLed], LOW);
        paused = true;
  }
    // Synchronization coded but disabled for now
   // iterateImage(imageData);*/
}

// Called by the light sensor interrupt
void startRound() {
   currentColumn = COLUMNS-1;

// Currently displayed shade
 currentLed = LEDS-1;
 unsigned long curStart = micros();
     //Serial.println("interuption!");
     if(lastRoundStart != 0) {
        unsigned long roundDelay = curStart - lastRoundStart;
        ledDelay = roundDelay / (COLUMNS * LEDS);
      }
    lastRoundStart = curStart;
    // Display one full round, stop if a new round is detected
    int numberOfLed = 0;
   while (numberOfLed < COLUMNS*LEDS) {
        iterateImage(imageData);
       numberOfLed++;
        if(isNewRound())
      {
        startRound();
        return;
      }
   }
}

