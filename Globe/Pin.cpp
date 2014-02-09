#include "Pin.h"

Pin::Pin(volatile uint8_t* port, volatile uint8_t* pin, volatile uint8_t* ddr, uint8_t index):
  port(port),
  pin(pin),
  ddr(ddr),
  index(index)
{}

void Pin::set(uint8_t value) const {
  if (value)
    *port = *port | (1 << index);
  else
    *port = *port & ~(1 << index);
}

uint8_t Pin::get() const {
  return (*pin >> index) & 1;
}

void Pin::makeOutput(uint8_t value) const {
  if (value)
    *ddr = *ddr | (1 << index);
  else
    *ddr = *ddr & (1 << index);
}