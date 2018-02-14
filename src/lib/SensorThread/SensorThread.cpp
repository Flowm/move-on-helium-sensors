#include "SensorThread.hpp"

void SensorThread::start() {
    if (setup()) {
        // Start thread if setup was successful
        _thread.start(callback(this, &SensorThread::loop));
    } else {
        printf("Setup of sensor was not successful. Not starting thread...\r\n");
    }
}

void SensorThread::loop() {
    while(1) {
        if (update_rate > 0) {
            Thread::wait(update_rate);
        }
        update();
    }
}

void SensorThread::set_update_rate(uint16_t update_rate) {
    this->update_rate = update_rate;
}

void SensorThread::set_priority(osPriority priority) {
    _thread.set_priority(priority);
}
