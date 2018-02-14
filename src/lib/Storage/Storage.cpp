#include "Storage.hpp"

Storage::Storage() : data(&packet.data) {
    packet.header.start = 0x01;
    packet.header.numBytes = sizeof(SensorData);
    packet.footer.end = 0x04;
}

void Storage::lock() {
    data_mutex.lock();
}

void Storage::unlock() {
    data_mutex.unlock();
}
