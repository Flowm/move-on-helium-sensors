/*
 * CDHUart.hpp
 *
 *  Created on: Dec 22, 2017
 *      Author: tkale
 */

#pragma once

#include <mbed.h>
#include <lib/MODSERIAL/MODSERIAL.h>
#include <lib/Checksum/Checksum.hpp>
#include <lib/Storage/Storage.hpp>

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

};

