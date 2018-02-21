#include "Storage.hpp"

Storage::Storage() : data(&packet.data) {
    packet.header.start = 0x01;
    packet.header.numBytes = sizeof(SensorData);
    packet.footer.end = 0x04;
}

void Storage::lock() {
    Timer t;
    t.start();
    data_mutex.lock();
    t.stop();
    packet.data.system.lock_wait_us = t.read_us();
}

void Storage::unlock() {
    data_mutex.unlock();
}
