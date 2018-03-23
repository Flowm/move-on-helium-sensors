#include "Sensors.hpp"

void Sensors::setup() {
    logger.printf("\r\nRESET\r\n");

    // Reset system time
    set_time(0);

    // Configure bus speeds
    spi.frequency(4000000);

    // Start sensor threads
    imu.start();
    env0.start();
    env1.start();
    adcs.start();
    toss.start();
    temperature.start();
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
    SensorData data_copy = *storage.data;
    SensorData* data = &data_copy;
    storage.unlock();

    // Lock serial and print all data
    logger.lock();
    logger.printf("GPS "
                  "LAT=%.6f,LON=%.6f,TIME=%u,SPEED=%.4f,TRUETRK=%.4f,ALT=%.4f"
                  "\r\n",
                  data->gps.lat, data->gps.lon, data->gps.timestamp,
                  data->gps.groundSpeed, data->gps.trueTrack, data->gps.altitude);

    for (int i = 0; i < MAX_ENV_SENSORS; i++) {
        logger.printf("ENV%d "
                      "TEMP=%.4f,HUM=%.4f,PRES=%.4f,GAS=%.4f"
                      "\r\n",
                      i,
                      data->env[i].temperature,
                      data->env[i].humidity,
                      data->env[i].pressure,
                      data->env[i].gasresistance);
    }


    if(temperature.getNumDevices() > 0) {
        logger.printf("TMP ");
        logger.printf("OW%d=%.4f", 0, data->temp[0].temp);
        for(int i = 1; i < temperature.getNumDevices(); i++) {
            logger.printf(",OW%d=%.4f", i, data->temp[i].temp);
        }
        logger.printf("\r\n");
    }

    logger.printf("TOSS RTC=%u",
                  data->toss.timestamp);
    for (int i = 0; i < 4; i++) {
        logger.printf(",TMP%u=%d", i+1, data->toss.temp[i]);
    }
    for (int i = 0; i < 9; i++) {
        logger.printf(",PH%u=%d", i+1, data->toss.photodiode[i]);
    }
        logger.printf("\r\n");

    logger.printf("SYS "
                  "LOG_CNT=%hu,"
                  "RTC_S=%hu,"
                  "LOG_MS=%hu,"
                  "LOCK_US=%u"
                  "\r\n",
                  data->system.log_cnt,
                  data->system.rtc_s,
                  data->system.log_ms,
                  data->system.lock_wait_us);

#if defined(MBED_STACK_STATS_ENABLED) && MBED_STACK_STATS_ENABLED == 1
    // Print memory stats if enabled
    print_all_thread_info();
    print_heap_and_isr_stack_info();
#endif

    logger.unlock();
}
