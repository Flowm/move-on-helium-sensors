/*
 * LoggerUart.hpp
 *
 *  Created on: Mar 6, 2018
 *      Author: tkale
 */

#pragma once

#include <mbed.h>

#define SYNC_BAUD 115200

/**
 * Provides a synchronous thread safe Serial class.
 */
class SyncSerial: public RawSerial {
public:
    SyncSerial(PinName tx, PinName rx, int baud = SYNC_BAUD) :
        RawSerial(tx,rx,baud) {};

    void lock() override;
    void unlock() override;

private:
    Mutex serial_mutex;
};

