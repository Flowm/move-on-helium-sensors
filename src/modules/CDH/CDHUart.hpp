/*
 * CDHUart.hpp
 *
 *  Created on: Dec 22, 2017
 *      Author: tkale
 */

#pragma once

#include <mbed.h>
#include <lib/MODSERIAL/MODSERIAL.h>


#define CDH_BAUD 115200

class CDHUart {
public:
    CDHUart(MODSERIAL &cdh) :
        cdh(cdh)
        {setup();};


private:
    //Interfaces
    MODSERIAL& cdh;

    //CDH Event Queue
    EventQueue queue;
    Thread queue_thread;

    //CDH protocol format.
    struct OutProtocolHeader {
        uint8_t opcode;
    };

    struct InProtocolHeader {
        uint8_t start;
        uint8_t status;
        uint16_t numBytes;
    };

    struct InProtocolFooter {
        uint8_t checksum;
        uint8_t end;
    };

    // Sensordata Structure.
    struct SensorData {
        uint16_t gyro[3];
    };

    struct CDHPacket {
        InProtocolHeader header;
        SensorData data;
        InProtocolFooter footer;
    };

    // Dummy Packet!
    CDHPacket data = {  {0x01,0xFE,sizeof(SensorData)},
                        {{'a','b','c'}},
                        {0xFE,0x04}};
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

