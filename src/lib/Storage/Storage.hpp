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

    /**
     * A shared timer to timestamp sensor data.
     * This will overflow but still should provide a source of
     * monotonicall increasing time as overflows can be detected
     * and corrected for later.
     */
    Timer timestamp;

private:
    Mutex data_mutex;
};
