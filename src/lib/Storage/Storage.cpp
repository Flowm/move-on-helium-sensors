#include "Storage.hpp"

Storage::Storage() : data(&packet.data) {
    packet.header.start = 0x01;
    packet.header.numBytes = sizeof(SensorData);
    packet.footer.end = 0x04;
    printf("SYS PACKET_SIZE=%u\r\n", sizeof(packet));
}

void Storage::lock() {
    Timer t;
    t.start();
    data_mutex.lock();
    t.stop();
    data->system.lock_wait_us = t.read_us();
}

void Storage::unlock() {
    data_mutex.unlock();
}

void Storage::update_log_stats(uint16_t log_ms) {
    data->system.log_cnt++;
    data->system.rtc_s = time(NULL);
    data->system.log_ms = log_ms;
}
