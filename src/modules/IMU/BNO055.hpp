#pragma once

#include <mbed.h>
#include <modules/SensorThread/SensorThread.hpp>
#include <modules/Storage/Storage.hpp>

/**
 * Driver for the BNO055 IMU
 */
class BNO055 : public SensorThread {
public:
    BNO055(I2C &i2c, Storage* storage) :
        i2c(i2c),
        storage(storage)
        {};

    void setup() override;
    void update() override;

    void getChipId();

private:
    I2C& i2c;
    Storage* storage;

    const int i2c_addr = 0x28 << 1;
    const int chip_id = 0xa0;
    char tx_buffer[8] = {};
    char rx_buffer[8] = {};
};
