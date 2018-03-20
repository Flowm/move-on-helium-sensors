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
    Timer t;
    t.start();

    while(1) {
        update();
        int time_passed = t.read_ms();
        int sleep = max(0, (_update_rate - time_passed));
        Thread::wait(sleep);
        t.reset();
    }
}

void SensorThread::set_update_rate(uint16_t update_rate) {
    _update_rate = update_rate;
}

void SensorThread::set_priority(osPriority priority) {
    _thread.set_priority(priority);
}
