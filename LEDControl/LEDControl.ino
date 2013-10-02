/**
 * LED controller
 * Displays a fixed image on the sphere.
 * No synchronization for now: fixed rounds per second.
 */

// Number of LEDs in the column
#define LEDS 39

// Number of colors per LED
#define COLORS 3

// Number of virtual columns in one round
#define COLUMNS 77

// Number of rounds per second
const int roundsPerSecond = 30;

// Positive LED pins
const byte ledPins[] = {A2,A3,A4,A5,A6,A7,A8,A9,A10,52,53,50,51,48,49,46,47,44,45,42,43,40,41,38,39,36,37,34,35,32,33,30,31,28,29,26,27,24,25};

// Negative color LED pins (red, green, blue)
const byte colorPins[] = {22,A0,23};

unsigned long logs[200];
int logId = 0;


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

// Number of shades (color combinations except black)
const int shades = (1 << COLORS) - 1;

// Delay to display one color of one column (microseconds)
unsigned long shadeDelay = 1000000 / (roundsPerSecond * COLUMNS * shades);

// Currently displayed column
int currentColumn = COLUMNS-1;

// Currently displayed shade
int currentShade = shades-1;

// Shade for which to start
// for each column, start at previously last shade
// to avoid LED changes
int currentShadeStart = 1;

// Date (microseconds) of the last LED refresh
unsigned long lastRefresh = 0;

// Current LED pins states
bool currentLedStates[LEDS];

// Current color pins states
bool currentColorStates[COLORS];

// Write to a PIN only if the current state is different
// from the last to save time.
// Changes the state accordingly.
void digitalWriteDiff(byte pin, bool *lastState, bool newState) {
    if (*lastState != newState) {
        digitalWrite(pin, newState);
        *lastState = newState;
    }
}

// Display given data column on the LEDs, for the given color
// Warning: to protect LEDS, they should never be on too long!
// The voltage is higher than the static limit!
void displayColumnShade(const byte data[COLUMNS][LEDS], int column, int shade) {
    int i;
    // Switch the previous shade off if different
    if (currentShade != shade) {
        for (i = 0; i < COLORS; i++)
            digitalWriteDiff(colorPins[i], &currentColorStates[i], HIGH);
    }
    // Switch the LEDs with the right shade on
    for (i = 0; i < LEDS; i++) {
        digitalWriteDiff(ledPins[i], &currentLedStates[i], data[column][i] == shade ? HIGH : LOW);
    }
    // Switch the proper shade on
    for (i = 0; i < COLORS; i++)
        digitalWriteDiff(colorPins[i], &currentColorStates[i], (shade & (1 << i)) == 0 ? HIGH : LOW);
    // Remember current display
    currentShade = shade;
    currentColumn = column;
    lastRefresh = micros();
}

// Refresh the column display based on the current state and on data
// Only if the delay since the last refresh is enough
boolean refreshImage(const byte data[COLUMNS][LEDS]) {
    unsigned long now = micros();
    unsigned long delay = now - lastRefresh;
    if (delay >= shadeDelay) {
        // Cycle through colors and columns
        // Repeating the color cycle for each column
        int newShade = currentShade + 1;
        if (newShade > shades)
            newShade = 1;
        int newColumn = currentColumn; 
        if (newShade == currentShadeStart)
        {
            // Start from the last displayed shade to avoid a change
            newShade = currentShade;
            currentShadeStart = currentShade;
            newColumn++;
            if (newColumn >= COLUMNS)
                newColumn = 0;
        }
        // Display
        displayColumnShade(data, newColumn, newShade);
        return true;
    }
    return false;
}

// Simple blocking version of refreshImage
void iterateImage(const byte data[COLUMNS][LEDS]) {
    while (!refreshImage(data));
}

// Full display with an unique shade during 3 seconds
void displayShade(byte shade) {
    unsigned long endTime = millis() + 3000;
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
    Serial.begin(9600);
    // LED pins initialization
    int i;
    for (i = 0; i < LEDS; i++) {
        pinMode(ledPins[i], OUTPUT);
        currentLedStates[i] = LOW;
    }
    for (i = 0; i < COLORS; i++) {
        pinMode(colorPins[i], OUTPUT);
        currentColorStates[i] = LOW;
    }
    // Test sequence (to check LEDs)
    delay(1000);
    Serial.print("Shade delay: ");
    Serial.println(shadeDelay);
    Serial.println("Test sequence");
    for (i = 0; i < COLORS; i++) {
        Serial.print("Color ");
        Serial.println(i);
        displayShade(1 << i);
    }
    Serial.println("End test sequence");
}

// Main method looping forever
void loop() {
    iterateImage(imageData);
}
