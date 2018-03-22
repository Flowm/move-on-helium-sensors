/*
 * DS18B20.hpp
 *
 *  Created on: Jan 15, 2018
 *      Author: tkale
 */

#pragma once

#include <mbed.h>
#include <lib/DS1820/DS1820.h>
#include <lib/SensorThread/SensorThread.hpp>

class DS18B20: public SensorThread {
public:
    DS18B20(PinName dataPin, Storage* storage, SyncSerial* logger):
        SensorThread(storage, logger, "DS18B20"),
        dataPin(dataPin)
        {};

    bool setup() override;
    void update() override;

    /**
     * Get the Number of devices connected on the OneWire bus.
     */
    uint8_t getNumDevices();

    ~DS18B20();
private:
    PinName dataPin;

    //Array to hold the individual sensors
    DS1820* sensors[MAX_TEMP_SENSORS];
    float temperature[MAX_TEMP_SENSORS];
    uint8_t numDevices = 0;
};

