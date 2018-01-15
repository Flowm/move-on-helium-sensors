/*
 * DS18B20.cpp
 *
 *  Created on: Jan 15, 2018
 *      Author: tkale
 */

#include <modules/Temperature/DS18B20.hpp>

void DS18B20::setup(){

    while(DS1820::unassignedProbe(dataPin)) {
        sensors[numDevices] = new DS1820(dataPin);
        numDevices++;
        if (numDevices == MAX_SENSORS)
            break;
    }
    sensors[0]->convertTemperature(false, DS1820::all_devices); //Start temperature conversion, wait until ready
    printf("Found %d device(s)\r\n\n", numDevices);
}

void DS18B20::print_all(){
    for (int i = 0; i< numDevices; i++)
        logger.printf("Device %d returns %foC\r\n", i, sensors[i]->temperature());
    printf("\r\n");
    sensors[0]->convertTemperature(false, DS1820::all_devices); //Start temperature conversion, wait until ready
}

DS18B20::~DS18B20(){
    for(uint8_t i = 0; i < numDevices; i++){
        free(sensors[i]);
    }
}
