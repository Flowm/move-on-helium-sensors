/*
 * Checksum.cpp
 *
 *  Created on: Jan 31, 2018
 *      Author: tkale
 */
#include "Checksum.hpp"

/**
 * Calculate CRC-16-CCITT in hardware
 *
 * @param [in] data A pointer to the data
 * @param [in] len  The length of the data
 *
 * WARNING: Doesnt seem to work!
 *
 * @return The CRC16 checksum of data
 *
 */
uint16_t Checksum::crc16hw(const uint8_t* data, uint16_t len)
{
    uint32_t checksum = 0x00;

    __HAL_RCC_CRC_CLK_ENABLE();
    CRC_HandleTypeDef crc;
    crc.InputDataFormat = CRC_INPUTDATA_FORMAT_HALFWORDS;
    crc.Init = {
            DEFAULT_POLYNOMIAL_DISABLE,
            DEFAULT_INIT_VALUE_DISABLE,
            0x1021,
            CRC_POLYLENGTH_16B,
            checksum,
            CRC_OUTPUTDATA_INVERSION_DISABLE
    };
    if(HAL_CRC_Init(&crc) == HAL_OK){
        checksum = HAL_CRC_Calculate(&crc, (uint32_t*)data, len);
    }else {
        printf("CRC INIT ERROR!");
    }
    HAL_CRC_DeInit(&crc);
    __HAL_RCC_CRC_CLK_DISABLE();
//    printf("%lu\r\n",checksum);
    return checksum;
}

/**
 * Calculate CRC-16-CCITT
 *
 * @param [in] data A pointer to the data
 * @param [in] len  The length of the data
 *
 * @return The CRC16 checksum of data
 *
 */
uint16_t Checksum::crc16sw(const uint8_t* data, uint16_t len)
{
    uint16_t crc = 0x0000;
    uint16_t x;

    while(len > 0) {
        x = crc >> 8 ^ *data++;
        x ^= x >> 4;
        crc = (crc << 8) ^ (x << 12) ^ (x << 5) ^ x;
        len--;
    }

    return crc;
}
