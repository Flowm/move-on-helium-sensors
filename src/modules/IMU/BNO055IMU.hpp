#pragma once

#include <mbed.h>
#include <lib/BNO055_fusion/BNO055.h>
#include <lib/SensorThread/SensorThread.hpp>
#include <lib/Storage/Storage.hpp>

/**
 * Driver for the BNO055 IMU
 */
class BNO055IMU : public SensorThread {
public:
    BNO055IMU(I2C &i2c, PinName reset_pin, Storage* storage) :
        SensorThread("BNO055"),
        i2c(i2c),
        storage(storage),
        impl(i2c, reset_pin)
        {};

    bool setup() override;
    void update() override;

    bool getChipId();

private:
    I2C& i2c;
    Storage* storage;

    BNO055 impl;

    BNO055_VECTOR_TypeDef accel;
    BNO055_VECTOR_TypeDef mag;
    BNO055_VECTOR_TypeDef gyro;
    BNO055_VECTOR_TypeDef gravity;
    BNO055_VECTOR_TypeDef lin_accel;
    BNO055_QUATERNION_TypeDef quaternion;
    BNO055_EULER_TypeDef angles;
    BNO055_TEMPERATURE_TypeDef temp;

    const int _chip_id = 0xa0;

    uint8_t reset_counter_temperatures = 0;
    uint8_t reset_counter_zeroes = 0;

    bool all_values_zero();
    uint8_t assemble_combined_resets();
};
