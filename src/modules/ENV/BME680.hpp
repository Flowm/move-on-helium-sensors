#pragma once

#include <mbed.h>
#include <lib/ClosedCube_BME680/src/ClosedCube_BME680_Mbed.h>
#include <lib/SensorThread/SensorThread.hpp>
#include <lib/Storage/Storage.hpp>

/**
 * Driver for the BME680 Environmental sensor
 */
class BME680 : public SensorThread {
public:
    BME680(SPI& spi, DigitalOut& cs, Storage* storage, int id=0) :
        bme(spi, cs),
        storage(storage),
        id(id)
        {};

    bool setup() override;
    void update() override;

private:
    ClosedCube_BME680_Mbed bme;
    Storage* storage;
    int id;

    const int _chip_id = 0x61;

    float _temperature;
    float _humidity;
    float _pressure;
    float _gasresistance;
};
