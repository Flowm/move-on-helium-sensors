#include "Sensors.hpp"

void Sensors::setup() {
    logger.printf("\r\nRESET\r\n");

    // Reset system time
    set_time(0);

    // Configure bus speeds
    spi.frequency(4000000);

    // Start sensor threads
    temperature.start();
    imu.start();
    env0.start();
    env1.start();
    adcs.start();
    toss.start();
    gps.start();

    // Wait for sensor threads to start and gather inital data
    Thread::wait(3500);
}

void Sensors::loop() {
    uint16_t log_ms = 0;
    Timer t;
    t.start();

    while(1) {
        t.reset();
        log(log_ms);
        log_ms = t.read_ms();
        int sleep = max(0, (1000 - 15 - log_ms));
        Thread::wait(sleep);
    }
}

void Sensors::log(uint16_t log_ms = 0) {
    // Copy data structure to avoid locking it during printf
    storage.lock();
    storage.update_log_stats(log_ms);
    storage.set_sensor_status(check_sensor_status());
    SensorData data_copy = *storage.data;
    SensorData* data = &data_copy;
    storage.unlock();

    // Lock serial and print all data
    logger.lock();
    logger.printf("SYS T=%lu"
                  "LOG_CNT=%hu,"
                  "RTC_S=%hu,"
                  "RTC_MS=%hu,"
                  "LOG_MS=%hu,"
                  "LOCK_US=%u,"
                  "STATUS=%hu"
                  "\r\n",
                  storage.get_ts(),
                  data->system.log_cnt,
                  data->system.rtc_s,
                  data->system.rtc_ms,
                  data->system.log_ms,
                  data->system.lock_wait_us,
                  data->system.sensor_status);

#if defined(MBED_STACK_STATS_ENABLED) && MBED_STACK_STATS_ENABLED == 1
    // Print memory stats if enabled
    print_all_thread_info();
    print_heap_and_isr_stack_info();
#endif

    logger.unlock();
}

uint8_t Sensors::check_sensor_status() {
    uint8_t status = 1 << 7 |
                     imu.is_valid() << 6 |
                     env0.is_valid() << 5 |
                     env1.is_valid() << 4 |
                     adcs.is_valid() << 3 |
                     toss.is_valid() << 2 |
                     temperature.is_valid() << 1 |
                     gps.is_valid() << 0;
    return ~status;
}
