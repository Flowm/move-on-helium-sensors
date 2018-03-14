/*
 * DS18B20.cpp
 *
 *  Created on: Jan 15, 2018
 *      Author: tkale
 */

#include <modules/Temperature/DS18B20.hpp>

bool DS18B20::setup() {

    // Update once every 2 seconds. Since the sensors need ~700 ms to convert
    // one temperature.
    set_update_rate(2000);

    while(DS1820::unassignedProbe(dataPin)) {
        sensors[numDevices] = new DS1820(dataPin);
        numDevices++;
        if (numDevices == MAX_TEMP_SENSORS)
            break;
    }

    sensors[0]->convertTemperature(false, DS1820::all_devices);

    printf("Found %d DS18B20 sensors\r\n", numDevices);

    return numDevices;
}

void DS18B20::update() {
    for (int i = 0; i< numDevices; i++) {
        //XXX: Disable interrupts to ensure onewire communication is not interrupted
        __disable_irq();
        temperature[i] = sensors[i]->temperature();
        // storage->data->temp[i].rom  = sensors[i]->getROM();
        __enable_irq();
    }

    storage->lock();
    for (int i = 0; i< numDevices; i++) {
        storage->data->temp[i].temp = temperature[i];
    }
    storage->unlock();

    // Start temperature conversion, wait until ready
    sensors[0]->convertTemperature(false, DS1820::all_devices);
}


uint8_t DS18B20::getNumDevices() {
    return numDevices;
}

DS18B20::~DS18B20() {
    for(uint8_t i = 0; i < numDevices; i++){
        delete(sensors[i]);
    }
}
