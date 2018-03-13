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
                  "LAT=%.6f,LON=%.6f,TIME=%u,SPEED=%.4f,TRUETRK=%.4f,MAGTRK=%.4f"
                  "\r\n",
                  data->gps.lat, data->gps.lon, data->gps.timestamp,
                  data->gps.groundSpeed, data->gps.trueTrack, data->gps.magTrack);

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

    logger.printf("IMU "
                  "ACC_X=%.4f,ACC_Y=%.4f,ACC_Z=%.4f,"
                  "GYRO_X=%.4f,GYRO_Y=%.4f,GYRO_Z=%.4f,"
                  "MAG_X=%.4f,MAG_Y=%.4f,MAG_Z=%.4f,"
                  //"QUAT_W=%.4f,QUAT_X=%.4f,QUAT_Y=%.4f,QUAT_Z=%.4f,"
                  "ANG_X=%.4f,ANG_Y=%.4f,ANG_Z=%.4f,"
                  "TEMP_ACC=%d,"//TEMP_GYRO=%d,"
                  "RSTS=%u"
                  "\r\n",
                  data->imu.accel.x, data->imu.accel.y, data->imu.accel.z,
                  data->imu.gyro.x, data->imu.gyro.y, data->imu.gyro.z,
                  data->imu.mag.x, data->imu.mag.y, data->imu.mag.z,
                  //data->imu.quaternion.w, data->imu.quaternion.x, data->imu.quaternion.y, data->imu.quaternion.z,
                  data->imu.orientation.x, data->imu.orientation.y, data->imu.orientation.z,
                  data->imu.temp_accel,//data->imu.temp_gyro,
                  data->imu.resets);

    logger.printf("ADCS "
                  //"ACC_X=%.4f,ACC_Y=%.4f,ACC_Z=%.4f,"
                  "GYRO_X=%.4f,GYRO_Y=%.4f,GYRO_Z=%.4f,"
                  "MAG_X=%.4f,MAG_Y=%.4f,MAG_Z=%.4f,"
                  "SUN_X=%.4f,SUN_Y=%.4f,SUN_Z=%.4f,"
                  "TEMP_OW1=%.4f,TEMP_OW2=%.4f,TEMP_OW3=%.4f,"
                  "SUN_RAW1=%hu,SUN_RAW2=%hu,SUN_RAW3=%hu,SUN_RAW4=%hu"
                  "\r\n",
                  //data->adcs.accel.x, data->adcs.accel.y, data->adcs.accel.z,
                  data->adcs.gyro.x, data->adcs.gyro.y, data->adcs.gyro.z,
                  data->adcs.mag.x, data->adcs.mag.y, data->adcs.mag.z,
                  data->adcs.sun.x, data->adcs.sun.y, data->adcs.sun.z,
                  data->adcs.temp[0], data->adcs.temp[1], data->adcs.temp[2],
                  data->adcs.raw_sun[0], data->adcs.raw_sun[1], data->adcs.raw_sun[2], data->adcs.raw_sun[3]);

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
