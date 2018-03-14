#include "SensorThread.hpp"

SensorThread::SensorThread(const char* name) :
    _thread(osPriorityNormal, OS_STACK_SIZE, NULL, name) {
    _name = name;
}

void SensorThread::start() {
    if (setup()) {
        // Start thread if setup was successful
        _thread.start(callback(this, &SensorThread::loop));
    } else {
        printf("Setup of %s sensor was not successful. Not starting thread...\r\n", _name);
    }
}

void SensorThread::loop() {
    while(1) {
        if (_update_rate > 0) {
            Thread::wait(_update_rate);
        }
        update();
    }
}

void SensorThread::set_update_rate(uint16_t update_rate) {
    _update_rate = update_rate;
}

void SensorThread::set_priority(osPriority priority) {
    _thread.set_priority(priority);
}
