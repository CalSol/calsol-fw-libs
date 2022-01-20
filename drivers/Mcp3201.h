#include <mbed.h>

#ifndef __MCP3201_H__
#define __MCP3201_H__


// MCP3201 12-bit SPI ADC
class Mcp3201 {
public:
  Mcp3201(SPI& spi, DigitalOut& cs);

  // reads ADC as a 12-bit value
  // frequency must be set externally, which also determines the sample rate
  uint16_t read_raw_u12();
  // reads ADC scaled up to a 16-bit value
  uint16_t read_u16();

protected:
  SPI& spi_;
  DigitalOut& cs_;
};

#endif  // __MCP3201_H__
