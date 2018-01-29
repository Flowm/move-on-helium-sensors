/*
 * GPS.hpp
 *
 *  Created on: Jan 21, 2018
 *      Author: tkale
 */

#pragma once

#include<mbed.h>

#define M8_ADDRESS  0x42
#define M8_DATALEN  0xFD
#define M8_DATA     0xFF


class GPS {
public:
    GPS(I2C &i2c):
        i2c(i2c)
        {setup();};
private:

    void readFromRegister(uint8_t address, uint8_t reg, char* data, uint8_t len, bool repeated);
    void setup();
    I2C& i2c;
};

