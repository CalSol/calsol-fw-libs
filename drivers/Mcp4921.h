#include <mbed.h>

#ifndef __MCP4921_H__
#define __MCP4921_H__


// MCP4921 12-bit SPI DAC
class Mcp4921 {
public:
  Mcp4921(SPI& spi, DigitalOut& cs, int frequency = 1000000);

  // writes a 12-bit value to the DAC
  // LDAC must be set externally
  void write_raw_u12(uint16_t value);
  // writes DAC scaled down from a 16-bit value
  void write_u16(uint16_t value);

protected:
  SPI& spi_;
  DigitalOut& cs_;
  int frequency_;
};

#endif  // __MCP4921_H__
