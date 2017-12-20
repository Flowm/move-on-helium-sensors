#pragma once

#include <mbed.h>

/**
 * Driver for the BME680 Environmental sensor
 */
class BME680 {
public:
    BME680(SPI &spi, DigitalOut &cs) :
        spi(spi),
        cs(cs)
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
    SPI& spi;
    DigitalOut& cs;

    const int chip_id = 0x61;
    char tx_buffer[8] = {};
    char rx_buffer[8] = {};
};
