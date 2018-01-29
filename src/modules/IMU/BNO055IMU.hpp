#pragma once

#include <mbed.h>
#include <lib/BNO055_fusion/BNO055.h>
#include <modules/SensorThread/SensorThread.hpp>
#include <modules/Storage/Storage.hpp>

/**
 * Driver for the BNO055 IMU
 */
class BNO055IMU : public SensorThread {
public:
    BNO055IMU(I2C &i2c, Storage* storage) :
        i2c(i2c),
        storage(storage),
        impl(i2c, D12)
        {};

    bool setup() override;
    void update() override;

    bool getChipId();

private:
    I2C& i2c;
    Storage* storage;

    BNO055 impl;

    const int _chip_id = 0xa0;
    char tx_buffer[8] = {};
    char rx_buffer[8] = {};
};
