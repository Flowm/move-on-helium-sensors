/*
 * Checksum.hpp
 *
 *  Created on: Jan 31, 2018
 *      Author: tkale
 */

#pragma once
#include <mbed.h>

class Checksum
{
public:
    static uint16_t crc16hw(const uint8_t* data, uint16_t len);
    static uint16_t crc16sw(const uint8_t* data, uint16_t len, uint16_t init = 0x0000);
};
