/*
 * DS18B20.hpp
 *
 *  Created on: Jan 15, 2018
 *      Author: tkale
 */

#pragma once

#include <mbed.h>
#include <lib/DS1820/DS1820.h>

#define MAX_SENSORS 16


class DS18B20 {
public:
    DS18B20(PinName dataPin, Serial& logger):
    dataPin(dataPin),
    logger(logger)
    {setup();};

    void print_all();
    ~DS18B20();
private:
    PinName dataPin;
    Serial& logger; // For debugging. Normal printf doesnt support floats!

    //Array to hold the individual sensors
    DS1820* sensors[MAX_SENSORS];
    uint8_t numDevices = 0;
    void setup();

};

