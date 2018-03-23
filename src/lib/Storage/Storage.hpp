#pragma once

#include <mbed.h>
#include <include/protocol.h>

class Storage {
public:
    Storage();

    void lock();
    void unlock();

    void update_log_stats(uint16_t log_ms);

    /**
     * Shared timer to timestamp sensor data.
     * This will overflow but still should provide a source of
     * monotonically increasing time as overflows can be detected
     * and corrected later.
     */
    uint32_t get_ts();

    CDHPacket packet;
    SensorData* const data;

private:
    Mutex data_mutex;
    Timer timestamp;
};
