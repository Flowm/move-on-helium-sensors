/*
 * CDHUart.hpp
 *
 *  Created on: Dec 22, 2017
 *      Author: tkale
 */

#pragma once

#include<mbed.h>

class CDHUart {
public:
    CDHUart(Serial &cdh) :
        cdh(cdh)
        {setup();};
    void transmitData();
private:
    //Interfaces
    Serial& cdh;

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
    struct SensorData {
        uint16_t gyro[3];
    };
    struct CDHPacket {
        InProtocolHeader header;
        SensorData data;
        InProtocolFooter footer;
    };

    void setup();

};

