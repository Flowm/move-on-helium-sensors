/*
 * LoggerUart.cpp
 *
 *  Created on: Mar 6, 2018
 *      Author: tkale
 */

#include <SyncSerial/SyncSerial.hpp>

void SyncSerial::lock() {
    serial_mutex.lock();
}

void SyncSerial::unlock() {
    serial_mutex.unlock();
}
