/*
 * CDHUart.hpp
 *
 *  Created on: Dec 22, 2017
 *      Author: tkale
 */

#pragma once

#include<mbed.h>
#include<MODSERIAL/MODSERIAL.h>


#define CDH_BAUD 115200
#define COMMAND_BUF_LEN 10

class CDHUart {
public:
    CDHUart(MODSERIAL &cdh) :
        cdh(cdh)
        {setup();};

    /**
     * Callback to process recieved opcodes from CDH
     *
     */
    void rxCallback(MODSERIAL_IRQ_INFO*);

    /**
     * Puts data out to CDH if any data has been requested.
     */
    void transmitData();

private:
    //Interfaces
    MODSERIAL& cdh;

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

    uint8_t opCodes[COMMAND_BUF_LEN] = {0};

    // Dummy Packet!
    CDHPacket data = {  {0x01,0xFE,sizeof(SensorData)},
                        {'a','b','c'},
                        {0xFE,0x04}};
    uint8_t numCommands = 0;
    void setup();

    /**
     * Calculates a simple XOR checksum over the SensorData and stores
     * it in the footer checksum field.
     * @param The CDHPacket for which the checksum is to be calculated.
     */
    void calculateChecksum(CDHPacket&);

};

