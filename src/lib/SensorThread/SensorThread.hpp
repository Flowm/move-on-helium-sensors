#pragma once

#include <mbed.h>
#include <algorithm>
#include <lib/Storage/Storage.hpp>
#include <lib/SyncSerial/SyncSerial.hpp>

//#define DEBUG_THREAD_TIMES

class SensorThread {
public:
    SensorThread(Storage* storage, SyncSerial* logger, const char* name = NULL);

    /**
     * Start thread
     */
    void start();

    /**
     * Thread loop func
     */
    void loop();

    /**
     * Inital sensor config
     */
    virtual bool setup() = 0;

    /**
     * Read sensor data and update datastructure
     */
    virtual void update() = 0;

    /**
     * Set update rate in ms for the thread loop.
     * A value of 0 causes a continously expecting a wait as part of the update
     * function.
     */
    void set_update_rate(uint16_t update_rate);

    /**
     * Set the thread scheduling priority
     */
    void set_priority(osPriority priority);

    /**
     * Indicate whether the sensor status is valid
     */
    bool is_valid();

    /**
     * Log to SD card
     */
    virtual void print() = 0;

protected:
    Storage* storage;
    SyncSerial* logger;
    bool valid = false;

private:
    Thread _thread;
    const char* _name;

    uint16_t _update_rate = 1000;
};
