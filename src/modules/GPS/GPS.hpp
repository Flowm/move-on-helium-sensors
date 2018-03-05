/*
 * GPS.hpp
 *
 *  Created on: Jan 21, 2018
 *      Author: tkale
 */

#pragma once

#include <mbed.h>
#include <time.h>
#include <lib/SensorThread/SensorThread.hpp>
#include <lib/minmea.git/minmea.h>
#include <lib/Storage/Storage.hpp>

#define M8_ADDRESS  (0x42 << 1)
#define M8_DATALEN  0xFD // 2 bytes 0xFD,0xFE
#define M8_DATA     0xFF
#define BUF_LEN     2000
#define CHUNK_LEN   80



class GPS : public SensorThread {
public:
    GPS(I2C* i2c, Storage* storage):
        i2c(i2c),
        storage(storage)
        {setup();};
private:

    I2C* i2c;
    Storage *storage;

    bool setup() override;
    void update() override;

    void readFromRegister(uint16_t device, uint16_t reg, char* data, uint32_t len, bool repeated);
    void callback(int event);

    uint16_t getDataLength();

    void processBuffer();
    void getNextChunk(uint16_t len);

    char data[BUF_LEN + 1];
    bool t_flag = true;
    minmea_sentence_gll gll;
    minmea_sentence_rmc rmc;
//    uint8_t readRegister(uint8_t address);
//    char* getNMEAData();
};

