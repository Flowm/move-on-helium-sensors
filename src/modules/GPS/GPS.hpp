/*
 * GPS.hpp
 *
 *  Created on: Jan 21, 2018
 *      Author: tkale
 */

#pragma once

#include<mbed.h>

#define M8_ADDRESS  (0x42<<1)
#define M8_DATALEN  0xFD // 2 bytes 0xFD,0xFE
#define M8_DATA     0xFF


class GPS {
public:
    GPS(I2C* i2c):
        i2c(i2c)
        {setup();};
private:

    I2C* i2c;
    char buffer[100];

    void setup();
    void readFromRegister(uint8_t device, uint8_t reg, char* data, uint8_t len, bool repeated);

//    uint8_t readRegister(uint8_t address);
//    char* getNMEAData();
};

