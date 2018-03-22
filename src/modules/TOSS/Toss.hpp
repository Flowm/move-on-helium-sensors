#pragma once

#include <mbed.h>
#include <lib/Checksum/Checksum.hpp>
#include <lib/SensorThread/SensorThread.hpp>
#include <lib/Storage/Storage.hpp>
#include <lib/SyncSerial/SyncSerial.hpp>

//#define TOSS_RAW_DATA

/**
 * Driver for the TOSS board
 */
class Toss : public SensorThread {
public:
    Toss(SPI* spi, DigitalOut* cs, Storage* storage, SyncSerial* logger) :
        SensorThread(storage, logger, "TOSS"),
        spi(spi),
        cs(cs)
        {};

    bool setup() override;
    void update() override;

private:
    SPI* spi;
    DigitalOut* cs;

    // Additional size in receive buffer to copensate for inital spi delay and
    // to ensure data sync
    char recv[sizeof(SensorTOSS)+20];
    SensorTOSS* data = (SensorTOSS*) &recv[1];
};
