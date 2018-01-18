#pragma once

#include <mbed.h>
#include "ClosedCube_BME680/src/ClosedCube_BME680_Mbed.h"

/**
 * Driver for the BME680 Environmental sensor
 */
class BME680 {
public:
    BME680(SPI &spi, DigitalOut &cs) :
        bme(spi, cs)
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
    ClosedCube_BME680_Mbed bme;

    const int _chip_id = 0x61;

    float _temperature;
    float _humidity;
    float _pressure;
    float _gasresistance;
};
