#include "Mcp3201.h"


Mcp3201::Mcp3201(SPI& spi, DigitalOut& cs) : spi_(spi), cs_(cs) {
  cs_ = 1;
}

// reads ADC as a 12-bit value
// frequency must be set externally, which also determines the sample rate
uint16_t Mcp3201::read_raw_u12() {
  spi_.format(8, 0);
  cs_ = 0;
  // first two clocks are for sampling, then one null bit, then data
  // and last bit (in a 16 bit transfer) is unused
  uint8_t byte0 = spi_.write(0);
  uint8_t byte1 = spi_.write(0);
  cs_ = 1;
  return (((uint16_t)(byte0 & 0x1f) << 8) | byte1) >> 1;
}  

// reads ADC scaled up to a 16-bit value
uint16_t Mcp3201::read_u16() {
  return (uint32_t)read_raw_u12() * 65535 / 4095;
}
