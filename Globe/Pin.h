/**
 * Utility to simplify the use of dynamic
 * input and output pins.
 * The main class is Pin, but it is abstract.
 * Classes to use are PinA, PinB, ... for each port.
 * All methods are inline in an attempt to optimize speed.
 */
#ifndef PORTS_H
#define PORTS_H

#include <avr/io.h>

class Pin {
private:

  // Index of the PIN
  uint8_t index;

protected:

  /**
   * Read the PORT register
   */
  uint8_t getPORT() const {
    return 0;
  }

  /**
   * Write the PORT register
   */
  void setPORT(uint8_t value) const {
  }

  /**
   * Read the PIN register
   */
  uint8_t getPIN() const {
    return 0;
  }

  /**
   * Write the PIN register
   */
  void setPIN(uint8_t value) const {
  }

  /**
   * Read the DDR register
   */
  uint8_t getDDR() const {
    return 0;
  }

  /**
   * Write the DDR register
   */
  void setDDR(uint8_t value) const {
  }

public:

  /**
   * Constructor: index of the PIN
   */
  Pin(uint8_t index) :
    index(index)
  {
  }

  /**
   * Set the value of a PIN.
   * Can also be used to set the pullup on an input
   */
  void set(uint8_t value) const {
    if (value)
      setPORT(getPORT() | (1 << index));
    else
      setPORT(getPORT() & ~(1 << index));
  }

  /**
   * Get the value of a PIN
   */
  uint8_t get() const {
    return (getPIN() >> index) & 1;
  }

  /**
   * Make a PIN an output or input
   */
  void makeOutput(uint8_t value) const {
    if (value)
      setDDR(getDDR() | (1 << index));
    else
      setDDR(getDDR() & ~(1 << index));
  }
};


/**
 * Define one class per port to avoid
 * indirect references
 */

class PinA : public Pin {
public:
  PinA(uint8_t index) : Pin(index) {
  }
protected:
  virtual uint8_t getPORT() const {
    return PORTA;
  }
  virtual void setPORT(uint8_t value) const {
    PORTA = value;
  }
  virtual uint8_t getPIN() const {
    return PINA;
  }
  virtual void setPIN(uint8_t value) const {
    PINA = value;
  }
  virtual uint8_t getDDR() const {
    return DDRA;
  }
  virtual void setDDR(uint8_t value) const {
    DDRA = value;
  }
};

class PinB : public Pin {
public:
  PinB(uint8_t index) : Pin(index) {
  }
protected:
  virtual uint8_t getPORT() const {
    return PORTB;
  }
  virtual void setPORT(uint8_t value) const {
    PORTB = value;
  }
  virtual uint8_t getPIN() const {
    return PINB;
  }
  virtual void setPIN(uint8_t value) const {
    PINB = value;
  }
  virtual uint8_t getDDR() const {
    return DDRB;
  }
  virtual void setDDR(uint8_t value) const {
    DDRB = value;
  }
};

class PinC : public Pin {
public:
  PinC(uint8_t index) : Pin(index) {
  }
protected:
  virtual uint8_t getPORT() const {
    return PORTC;
  }
  virtual void setPORT(uint8_t value) const {
    PORTC = value;
  }
  virtual uint8_t getPIN() const {
    return PINC;
  }
  virtual void setPIN(uint8_t value) const {
    PINC = value;
  }
  virtual uint8_t getDDR() const {
    return DDRC;
  }
  virtual void setDDR(uint8_t value) const {
    DDRC = value;
  }
};

class PinD : public Pin {
public:
  PinD(uint8_t index) : Pin(index) {
  }
protected:
  virtual uint8_t getPORT() const {
    return PORTD;
  }
  virtual void setPORT(uint8_t value) const {
    PORTD = value;
  }
  virtual uint8_t getPIN() const {
    return PIND;
  }
  virtual void setPIN(uint8_t value) const {
    PIND = value;
  }
  virtual uint8_t getDDR() const {
    return DDRD;
  }
  virtual void setDDR(uint8_t value) const {
    DDRD = value;
  }
};

class PinE : public Pin {
public:
  PinE(uint8_t index) : Pin(index) {
  }
protected:
  virtual uint8_t getPORT() const {
    return PORTE;
  }
  virtual void setPORT(uint8_t value) const {
    PORTE = value;
  }
  virtual uint8_t getPIN() const {
    return PINE;
  }
  virtual void setPIN(uint8_t value) const {
    PINE = value;
  }
  virtual uint8_t getDDR() const {
    return DDRE;
  }
  virtual void setDDR(uint8_t value) const {
    DDRE = value;
  }
};

class PinF : public Pin {
public:
  PinF(uint8_t index) : Pin(index) {
  }
protected:
  virtual uint8_t getPORT() const {
    return PORTF;
  }
  virtual void setPORT(uint8_t value) const {
    PORTF = value;
  }
  virtual uint8_t getPIN() const {
    return PINF;
  }
  virtual void setPIN(uint8_t value) const {
    PINF = value;
  }
  virtual uint8_t getDDR() const {
    return DDRF;
  }
  virtual void setDDR(uint8_t value) const {
    DDRF = value;
  }
};

class PinG : public Pin {
public:
  PinG(uint8_t index) : Pin(index) {
  }
protected:
  virtual uint8_t getPORT() const {
    return PORTG;
  }
  virtual void setPORT(uint8_t value) const {
    PORTG = value;
  }
  virtual uint8_t getPIN() const {
    return PING;
  }
  virtual void setPIN(uint8_t value) const {
    PING = value;
  }
  virtual uint8_t getDDR() const {
    return DDRG;
  }
  virtual void setDDR(uint8_t value) const {
    DDRG = value;
  }
};

class PinH : public Pin {
public:
  PinH(uint8_t index) : Pin(index) {
  }
protected:
  virtual uint8_t getPORT() const {
    return PORTH;
  }
  virtual void setPORT(uint8_t value) const {
    PORTH = value;
  }
  virtual uint8_t getPIN() const {
    return PINH;
  }
  virtual void setPIN(uint8_t value) const {
    PINH = value;
  }
  virtual uint8_t getDDR() const {
    return DDRH;
  }
  virtual void setDDR(uint8_t value) const {
    DDRH = value;
  }
};

class PinJ : public Pin {
public:
  PinJ(uint8_t index) : Pin(index) {
  }
protected:
  virtual uint8_t getPORT() const {
    return PORTJ;
  }
  virtual void setPORT(uint8_t value) const {
    PORTJ = value;
  }
  virtual uint8_t getPIN() const {
    return PINJ;
  }
  virtual void setPIN(uint8_t value) const {
    PINJ = value;
  }
  virtual uint8_t getDDR() const {
    return DDRJ;
  }
  virtual void setDDR(uint8_t value) const {
    DDRJ = value;
  }
};

class PinK : public Pin {
public:
  PinK(uint8_t index) : Pin(index) {
  }
protected:
  virtual uint8_t getPORT() const {
    return PORTK;
  }
  virtual void setPORT(uint8_t value) const {
    PORTK = value;
  }
  virtual uint8_t getPIN() const {
    return PINK;
  }
  virtual void setPIN(uint8_t value) const {
    PINK = value;
  }
  virtual uint8_t getDDR() const {
    return DDRK;
  }
  virtual void setDDR(uint8_t value) const {
    DDRK = value;
  }
};

class PinL : public Pin {
public:
  PinL(uint8_t index) : Pin(index) {
  }
protected:
  virtual uint8_t getPORT() const {
    return PORTL;
  }
  virtual void setPORT(uint8_t value) const {
    PORTL = value;
  }
  virtual uint8_t getPIN() const {
    return PINL;
  }
  virtual void setPIN(uint8_t value) const {
    PINL = value;
  }
  virtual uint8_t getDDR() const {
    return DDRL;
  }
  virtual void setDDR(uint8_t value) const {
    DDRL = value;
  }
};

#endif