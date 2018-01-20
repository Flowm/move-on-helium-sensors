#pragma once

#include <mbed.h>
#include <modules/SensorThread/SensorThread.hpp>
#include <modules/Storage/Storage.hpp>

#include "ClosedCube_BME680/src/ClosedCube_BME680_Mbed.h"

/**
 * Driver for the BME680 Environmental sensor
 */
class BME680 : public SensorThread {
public:
    BME680(SPI& spi, DigitalOut& cs, Storage* storage) :
        bme(spi, cs),
        storage(storage)
        {};

    void setup() override;
    void update() override;

private:
    ClosedCube_BME680_Mbed bme;
    Storage* storage;

    const int _chip_id = 0x61;

    float _temperature;
    float _humidity;
    float _pressure;
    float _gasresistance;
};
