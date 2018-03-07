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
class SyncSerial: public Serial {
public:
    SyncSerial(PinName tx, PinName rx, int baud = SYNC_BAUD) :
        Serial(tx,rx,baud){};

protected:
    void lock() override;
    void unlock() override;
private:
    Mutex serial_mutex;
};

