/**
 * PCF2129.cpp
 *
 * Created on: Oct 7, 2016
 *     Author: Avinash
 */

#include "PCF2129.h"

static uint8_t intToBcd(uint8_t in) {
  return (((in / 10) % 10) << 4)
      | (in % 10);
}

static uint8_t bcdToInt(uint8_t in) {
  return ((in >> 4) & 0xf) * 10
      + (in & 0xf);
}

uint8_t PCF2129_I2C::read_i2c_reg(uint8_t base) {

    bool success = false;
    uint8_t data[1] = { base };
    int status = RTC_i2c->write(0b10100010, (const char*)data, sizeof(data), false);
    if (status == 0) {
        status = RTC_i2c->read(0b10100011, (char*)data, sizeof(data));
        success = (status == 0);
        return data[0];
    }
}

bool PCF2129_I2C::gettime(tm *time) {

    // int address = 0b10100011;           //I2C-bus sub address + read logic 1

    uint8_t sec = read_i2c_reg(0x03);
    uint8_t min = read_i2c_reg(0x04);
    uint8_t hour = read_i2c_reg(0x05);
    uint8_t day = read_i2c_reg(0x06);
    uint8_t weekday = read_i2c_reg(0x07);
    uint8_t month = read_i2c_reg(0x08);
    uint8_t year = read_i2c_reg(0x09);

    (void)(weekday);  // avoid unused variable warning

    // Convert from device's BCD (binary coded decimal) format to int
    time->tm_sec = bcdToInt(sec & 0x7f);
    time->tm_min = bcdToInt(min & 0x7f);
    time->tm_hour = bcdToInt(hour & 0x3f);
    time->tm_mday = bcdToInt(day & 0x3f);
    time->tm_mon = bcdToInt(month & 0x1f) - 1;
    time->tm_year = bcdToInt(year) + 100;
    return !(sec & 0x80);  // OSF bit
}

void PCF2129_I2C::settime(const tm& time) {
    uint8_t address = 0b10100010;           //I2C-bus sub address + write logic 0

    uint8_t values[7] = {intToBcd(time.tm_sec), intToBcd(time.tm_min), intToBcd(time.tm_hour),
    intToBcd(time.tm_mday), intToBcd(time.tm_wday), intToBcd(time.tm_mon) + 1, intToBcd(time.tm_year)};

    uint8_t counter = 0;
    for (uint8_t base=0x03; base <= 0x09; base++) {
        uint8_t data[2] = { base, values[counter] };
        RTC_i2c->write(address, (const char*)data, sizeof(data));
        counter++;
    }
}

bool PCF2129_SPI::gettime(tm *time) {
    int command = 0b10100000;           //Read starting at the sec register
    *RTC_cs = 0;
    wait_us(1);
    /** Write in command, then read the registers holding the time componenets */
    RTC_spi->write(command | 0x03);
    uint8_t sec = RTC_spi->write(0x00);
    uint8_t min = RTC_spi->write(0x00);
    uint8_t hour = RTC_spi->write(0x00);
    uint8_t day = RTC_spi->write(0x00);
    uint8_t weekday = RTC_spi->write(0x00);
    uint8_t month = RTC_spi->write(0x00);
    uint8_t year = RTC_spi->write(0x00);
    *RTC_cs = 1;

    (void)(weekday);  // avoid unused variable warning

    // Convert from device's BCD (binary coded decimal) format to int
    time->tm_sec = bcdToInt(sec & 0x7f);
    time->tm_min = bcdToInt(min & 0x7f);
    time->tm_hour = bcdToInt(hour & 0x3f);
    time->tm_mday = bcdToInt(day & 0x3f);
    time->tm_mon = bcdToInt(month & 0x1f) - 1;
    time->tm_year = bcdToInt(year) + 100;
    return !(sec & 0x80);  // OSF bit
}

void PCF2129_SPI::settime(const tm &time) {
    uint8_t command = 0b00100000;           //Write starting at the sec register
    // Convert to device's BCD (binary coded decimal) format
    uint8_t sec = intToBcd(time.tm_sec);
    uint8_t min = intToBcd(time.tm_min);
    uint8_t hour = intToBcd(time.tm_hour);
    uint8_t day = intToBcd(time.tm_mday);
    uint8_t month = intToBcd(time.tm_mon) + 1;
    uint8_t year = intToBcd(time.tm_year);

    /** Write this out to the RTC */
    *RTC_cs = 0;
    wait_us(1);
    RTC_spi->write(command | 0x03);
    RTC_spi->write(sec);
    RTC_spi->write(min);
    RTC_spi->write(hour);
    RTC_spi->write(day);
    RTC_spi->write(0);
    RTC_spi->write(month);
    RTC_spi->write(year);
    *RTC_cs = 1;
}

void PCF2129_SPI::enable_s_int() {
    /** Set the configuration to enable the second interrupt */
    int command = 0b00100000;
    *RTC_cs=0;
    wait_us(1);
    RTC_spi->write(command | 0x10);
    RTC_spi->write(1<<5);
    *RTC_cs=1;
    wait_us(5);
    *RTC_cs=0;
    wait_us(1);
    RTC_spi->write(command | 0x00);
    RTC_spi->write(0x01);
    *RTC_cs=1;
}
