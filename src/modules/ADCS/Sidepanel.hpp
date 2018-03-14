#pragma once

#include <mbed.h>
#include <include/adcs_protocol.h>
#include <lib/Checksum/Checksum.hpp>
#include <lib/SensorThread/SensorThread.hpp>
#include <lib/Storage/Storage.hpp>
#include <lib/SyncSerial/SyncSerial.hpp>

#define SIDEPANEL_PRINT_CHECKSUM_ERRORS
//#define SIDEPANEL_RAW_DATA

/**
 * Driver for the ADCS Sidepanel
 */
class Sidepanel : public SensorThread {
public:
    Sidepanel(SPI* spi, DigitalOut* cs, Storage* storage, SyncSerial* logger) :
        SensorThread("ADCS"),
        spi(spi),
        cs(cs),
        storage(storage),
        logger(logger)
        {};

    bool setup() override;
    void update() override;

private:
    void updateVerifyData(const uint8_t* data, VerifyStruct* verify);
    bool checkVerifyData(const uint8_t* data, VerifyStruct* verify);

    SPI* spi;
    DigitalOut* cs;
    Storage* storage;
    SyncSerial* logger;

    // Additional size in receive buffer to copensate for inital spi delay and
    // to ensure data sync
    uint8_t recv[sizeof(SidepanelData) + 50];
    SidepanelData* data = (SidepanelData*) &recv[1];
    SidepanelControl control;
};
