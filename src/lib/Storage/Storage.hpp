#pragma once

#include <mbed.h>
#include <include/protocol.h>

class Storage {
public:
    Storage();

    void lock();
    void unlock();

    CDHPacket packet;
    SensorData* data;
private:
    Mutex data_mutex;
};