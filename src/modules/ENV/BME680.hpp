#pragma once

#include <mbed.h>
#include <lib/ClosedCube_BME680/src/ClosedCube_BME680_Mbed.h>
#include <lib/SensorThread/SensorThread.hpp>

/**
 * Driver for the BME680 Environmental sensor
 */
class BME680 : public SensorThread {
public:
    BME680(SPI* spi, DigitalOut* cs, int id, Storage* storage, SyncSerial* logger) :
        SensorThread(storage, logger, "BME680"),
        bme(spi, cs),
        _id(id)
        {};

    bool setup() override;
    void update() override;
    void print() override;

private:
    ClosedCube_BME680_Mbed bme;
    SensorENV data;
    int _id;

    const int _chip_id = 0x61;

    int _read_attempts = 0;

};
