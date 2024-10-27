/**
 * PCF2129.h
 *
 * Created on: Oct 7, 2016
 *     Author: Avinash
 */

#ifndef __PCF2129_H__
#define __PCF2129_H__

#include <mbed.h>
#include <time.h>

/** Class abstraction for the PCF2129 */
class PCF2129_Base {

public:

    /** Read the current time and put it in time.
     * RTC MUST BE SET IN 24 HOUR MODE, not 12 hour AM/PM mode.
     *
     * @param time Output pointer
     *
     * @return if the oscillator hasn't stopped (if the OSF bit is clear)
     */
    virtual bool gettime(tm *time);

    /** Set the time on the RTC from the datetime object
     * RTC MUST BE SET IN 24 HOUR MODE, not 12 hour AM/PM mode.
     *
     * @param time Time to set
     */
    virtual void settime(const tm& time);

    
};

class PCF2129_I2C : public PCF2129_Base {
	private:
		I2C* RTC_i2c;           // Pointer to I2C object that the RTC is connected to

    public:

        /** Constructor for PCF2129_I2C object
         *
         * @param r_i2c Reference to an I2C object connected to the RTC
         */
        PCF2129_I2C(I2C *r_i2c) : RTC_i2c(r_i2c) {
        }
        
        bool gettime(tm *time);
        void settime(const tm& time);
        uint8_t read_i2c_reg(uint8_t base);
};


class PCF2129_SPI : public PCF2129_Base {
	private:
		SPI* RTC_spi;           // Pointer to SPI object that the RTC is connected to
		DigitalOut* RTC_cs;     // Pointer to the DigitalOut object connected to the RTC CS pin
    
        /** Enable the second interrupt
         * The INT pin will go HIGH on every second and will attach
         */
        void enable_s_int();
    public:
        bool gettime(tm *time);
        void settime(const tm& time);
};

#endif // __PCF_2129_H__
