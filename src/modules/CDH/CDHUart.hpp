/*
 * CDHUart.hpp
 *
 *  Created on: Dec 22, 2017
 *      Author: tkale
 */

#pragma once

#include <mbed.h>
#include <lib/MODSERIAL/MODSERIAL.h>
#include <modules/Storage/Storage.hpp>


#define CDH_BAUD 115200

class CDHUart {
public:
    CDHUart(MODSERIAL &cdh, Storage* storage) :
        cdh(cdh),
        storage(storage)
        {setup();};


private:
    //Interfaces
    MODSERIAL& cdh;

    //CDH Event Queue
    EventQueue queue;
    Thread queue_thread;

    Storage* storage;

    // Dummy Packet!
    /*
     *CDHPacket data = {{0x01, 0xFE, sizeof(SensorData)},
     *                  {},
     *                  {0xFE,0x04}
     *                 };
     */

    void setup();

    /**
     * Calculates checksum and puts data out to CDH if any data has been requested.
     */
    void transmitData();

    /**
     * Called by the event queue to process the recieved command.
     * @param Command as recieved from CDH.
     */
    void processCommand(uint8_t);

    /**
     * ISR Callback to process recieved opcodes from CDH
     *
     */
    void rxCallback(MODSERIAL_IRQ_INFO*);

    /**
     * Calculates a simple XOR checksum over the SensorData and stores
     * it in the footer checksum field.
     * @param The CDHPacket for which the checksum is to be calculated.
     */
    void calculateChecksum(CDHPacket&);

};

