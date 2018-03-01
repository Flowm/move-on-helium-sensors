#pragma once

#include <mbed.h>
#include <include/protocol.h>

class Storage {
public:
    Storage();

    void lock();
    void unlock();

    void update_log_stats(uint16_t log_ms);

    CDHPacket packet;
    SensorData* const data;
private:
    Mutex data_mutex;
};
