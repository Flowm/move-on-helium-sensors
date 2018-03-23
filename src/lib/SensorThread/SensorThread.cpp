#include "SensorThread.hpp"

SensorThread::SensorThread(Storage* storage, SyncSerial* logger, const char* name) :
    storage(storage),
    logger(logger),
    _thread(osPriorityNormal, OS_STACK_SIZE, NULL, name) {
    _name = name;
}

void SensorThread::start() {
    if (setup()) {
        // Start thread if setup was successful
        _thread.start(callback(this, &SensorThread::loop));
    } else {
        logger->lock();
        logger->printf("Setup of %s sensor was not successful. Not starting thread...\r\n", _name);
        logger->unlock();
    }
}

void SensorThread::loop() {
    Timer t;
    t.start();

    while(1) {
        update();
        print();
        int time_passed = t.read_ms();
        int sleep_ms = max(0, (_update_rate - time_passed));
#ifdef DEBUG_THREAD_TIMES
        logger->lock();
        logger->printf("SYS %s_R=%d,%s_S=%d\r\n", _name, time_passed, _name, sleep_ms);
        logger->unlock();
#endif
        Thread::wait(sleep_ms);
        t.reset();
    }
}

void SensorThread::set_update_rate(uint16_t update_rate) {
    _update_rate = update_rate;
}

void SensorThread::set_priority(osPriority priority) {
    _thread.set_priority(priority);
}
