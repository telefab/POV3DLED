#if defined(__AVR_ATmega2560__)

#include "BusSlave.h"
#include <avr/interrupt.h>

BusSlave* BusSlave::singleton = 0;

// Pin 3 on the Arduino MEGA (INT5)
const PIN_TYPE BusSlave::busPin = PIN_E(5);

BusSlave::BusSlave() :
  dataReadPtr(0),
  dataWritePtr(0),
  availableBytes(0),
  currentBit(0),
  lastPulseDate(0),
  lastPulseDateSet(0),
  stringIndex(0),
  stringCharIndex(0),
  stringEnds(0)
{
}

BusSlave* BusSlave::get() {
  if (!BusSlave::singleton)
    BusSlave::singleton = new BusSlave();
  return BusSlave::singleton;
}

void BusSlave::begin() {
  // Prepare the bus PIN
  busPin.makeOutput(0);
  // Disable global interrupts
  cli();
  // Initialize the bus delay timer (Timer4, /8, 0.5us, compare interrupt when symbol is finished)
  // Not using ICP because interrupts have an overly high priority
  // and prevent the globe from working.
  TCCR4A = 0;
  TCCR4B = 0;
  TIMSK4 = 0;
  TCCR4B |= (1<<ICES4) | (1 << CS41);
  TIMSK4 |= (1 << OCIE4A);
  OCR4A = 0xFFFF;
  // Activate bus interrupts (INT5, rising)
  EIMSK |= (1 << INT5);
  EICRB |= (1 << ISC50) | (1 << ISC51); 
  // enable global interrupts
  sei();
}

uint16_t BusSlave::available() {
  return availableBytes;
}

char BusSlave::read(uint16_t& validBits) {
  if (availableBytes == 0)
    return 0;
  // Get the valid bits
  validBits = lengthFifo[dataReadPtr];
  return read();
}

char BusSlave::read() {
  if (availableBytes == 0)
    return 0;
  char result = dataFifo[dataReadPtr];
  availableBytes--;
  dataReadPtr++;
  if (dataReadPtr >= BUS_FIFO_SIZE)
    dataReadPtr = 0;
  return result;
}

uint8_t BusSlave::readString(char data[], uint16_t maxSize, uint8_t redundancy) {
  if (available() > 0) {
    char curByte = read();
    if (curByte == 0 && stringCharIndex == 0) {
      // Signal of an end of redundancy
      if (stringIndex > 0) {

      }
    } else {
      // Store the character if possible
      if (stringCharIndex < maxSize && stringIndex <= redundancy && (curByte != 0 || stringEnds == 0)) {
        data[stringIndex*maxSize + stringCharIndex] = curByte;
        stringCharIndex++;
      }
      if (curByte == 0) {
        // End of string character
        if (redundancy == 0) {
          // No redundancy, return
          stringIndex = 0;
          stringCharIndex = 0;
          data[maxSize-1] = 0;
          return 1;
        }
        stringEnds++;
      } else {
        // Normal character
        stringEnds = 0;
      }
      if (stringEnds == 3)
      {
        // End of string
        stringIndex++;
        stringCharIndex = 0;
        if (stringIndex > redundancy) {
          // This is the end of redundancy        
          uint8_t best = 0;
          uint8_t bestScore = 0;
          uint8_t score = 0;
          // Vote for each byte
          for (uint16_t i = 0; i < maxSize; i++) {
            for (uint8_t j = 0; j < stringIndex; j++) {
              for (uint8_t k = j+1; k < stringIndex; k++) {
                if (data[i+j*maxSize] == data[i+k*maxSize])
                  score++;
              }
              if (score > bestScore) {
                bestScore = score;
                best = j;
              }
            }
            data[i] = data[i+best*maxSize];
          }
          // Return
          stringIndex = 0;
          data[maxSize-1] = 0;
          return 1;
        }
      }
      // End of a string with redundancy
    }
  }
  return 0;
}

void BusSlave::saveSymbol(uint8_t symbol) {
  if (symbol < BUS_SYM_END) {
    // Received a normal bit symbol
    // Store the bit if there is some space
    if (availableBytes < BUS_FIFO_SIZE) {
      // Initialize the byte
      if (currentBit == 0)
        dataFifo[dataWritePtr] = 0;
      // Ignore the bit if a bit has not been signaled finished
      if (currentBit <= 7) {
        dataFifo[dataWritePtr] |= (symbol << currentBit);
        currentBit++;
      }
    }
  } else {
    // Received an end of byte
    // Ignore the byte if it is empty
    if (currentBit > 0) {
      // This byte is finished, and is an end byte
      lengthFifo[dataWritePtr] = currentBit;
      currentBit = 0;
      availableBytes++;
      dataWritePtr++;
      if (dataWritePtr >= BUS_FIFO_SIZE)
        dataWritePtr = 0;
    }
  }
}

uint16_t BusSlave::_measurePulseDelay(uint16_t date, uint8_t &first, uint8_t &noise) {
  if (lastPulseDateSet == 0) {
    // First pulse of a communication
    lastPulseDate = date;
    lastPulseDateSet = 1;
    first = 1;
    noise = 0;
    return 0;
  }
  first = 0;
  // Measure the delay
  uint16_t delay = date - lastPulseDate;
  if (delay >= BUS_SYMBOL_BASE) {
    lastPulseDate = date;
    noise = 0;
  } else {
    // Pulses too close: this is noise
    // Long pulses sent are received as a large number of very short pulses
    noise = 1;
  }
  return delay;
}

void BusSlave::_onReceivedPulse(uint16_t delay) {
  // Interpret the symbol and save
  uint8_t symbol = (delay - (BUS_SYMBOL_BASE+1)) / (BUS_SYMBOL_BASE*2);
  saveSymbol(symbol);
}

void BusSlave::_onSymbolTimeout() {
  if (lastPulseDateSet) {
    lastPulseDateSet = 0;
    saveSymbol(BUS_SYM_END);
  }
}

uint16_t _dateBusSlave, _delayBusSlave;
uint8_t _firstBusSlave, _noiseBusSlave;
BusSlave* _instBusSlave = BusSlave::get();

/**
 * Called at each bus rising edge
 */
ISR (INT5_vect)
{
  // Measure the delay
  _dateBusSlave = TCNT4;
  _delayBusSlave = _instBusSlave->_measurePulseDelay(_dateBusSlave, _firstBusSlave, _noiseBusSlave);
  if (!_noiseBusSlave) {
    // Reset the timer compare to detect an end pulse
    OCR4A = _dateBusSlave + (BUS_SYMBOL_BASE*(BUS_SYM_END+1)+BUS_SYMBOL_BASE/2)*2;
    // Analyze the pulse
    if (!_firstBusSlave)
      _instBusSlave->_onReceivedPulse(_delayBusSlave);
  }
}

/**
 * Called by the timer if a symbol is done
 */
ISR(TIMER4_COMPA_vect) {
  // Symbol finished
  _instBusSlave->_onSymbolTimeout();
}

#endif