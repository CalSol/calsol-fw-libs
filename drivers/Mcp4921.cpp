#include "Mcp4921.h"

  
Mcp4921::Mcp4921(SPI& spi, DigitalOut& cs, int frequency) : spi_(spi), cs_(cs), frequency_(frequency) {
  cs_ = 1;
}

void Mcp4921::write_raw_u12(uint16_t value) {
  spi_.frequency(frequency_);
  spi_.format(8, 0);
  cs_ = 0;
  // first 4 bits: write DACA, unbuffered mode, 1x gain, enable
  spi_.write(0x30 | ((value >> 8) & 0x0f));
  spi_.write(value & 0xff);
  cs_ = 1;
}  

void Mcp4921::write_u16(uint16_t value) {
  write_raw_u12((uint32_t)value * 4095 / 65535);
}
