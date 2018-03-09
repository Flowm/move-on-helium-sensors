#pragma once

#include <mbed.h>
#include <lib/SensorThread/SensorThread.hpp>
#include <lib/Storage/Storage.hpp>

/**
 * Driver for the ADCS Sidepanel
 */
class Sidepanel : public SensorThread {
public:
    Sidepanel(SPI& spi, DigitalOut& cs, Storage* storage) :
        spi(spi),
        cs(cs),
        storage(storage)
        {};

    bool setup() override;
    void update() override;

private:
    SPI& spi;
    DigitalOut& cs;
    Storage* storage;
};
