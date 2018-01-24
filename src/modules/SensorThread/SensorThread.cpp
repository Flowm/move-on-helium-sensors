#include "SensorThread.hpp"

void SensorThread::start() {
    setup();
    _thread.start(callback(this, &SensorThread::loop));
}

void SensorThread::loop() {
    while(1) {
        update();
        if (update_rate > 0) {
            Thread::wait(update_rate);
        }
    }
}

void SensorThread::set_update_rate(uint16_t update_rate) {
    this->update_rate = update_rate;
}

void SensorThread::set_priority(osPriority priority) {
    _thread.set_priority(priority);
}
