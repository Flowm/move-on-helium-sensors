#pragma once

#include <mbed.h>

/**
 * Driver for the BNO055 IMU
 */
class BNO055 {
public:
    BNO055(I2C &i2c) :
        i2c(i2c)
        {};

    /**
     * Inital sensor config
     */
    void setup();

    /**
     * Read sensor data and print it
     */
    void read();

private:
    I2C& i2c;

    const int i2c_addr = 0x28 << 1;
    const int chip_id = 0xa0;
    char tx_buffer[8] = {};
    char rx_buffer[8] = {};
};
