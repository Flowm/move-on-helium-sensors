#include "Sensors.hpp"

void Sensors::setup() {
    printf("\r\nRESET\r\n");
    set_time(0);
    imu.start();
    env0.start();
    env1.start();
    temperature.start();
}

void Sensors::loop() {
    // Wait for sensor threads to start and gather inital data
    Thread::wait(2500);
    Timer t;
    t.start();

    while(1) {
        log();
        int sleep = max(0, (1000 - 15 - t.read_ms()));
        //logger.printf("DBG WAIT=%d\r\n", sleep);
        Thread::wait(sleep);
        t.reset();
    }
}

void Sensors::log() {
    storage.lock();
    storage.update_log_stats();
    SensorData data_copy = *storage.data;
    SensorData* data = &data_copy;
    storage.unlock();

    for (int i = 0; i <= 1; i++) {
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
                  "MAG_X=%.4f,MAG_Y=%.4f,MAG_Z=%.4f,"
                  "GYRO_X=%.4f,GYRO_Y=%.4f,GYRO_Z=%.4f,"
                  "QUAT_W=%.4f,QUAT_X=%.4f,QUAT_Y=%.4f,QUAT_Z=%.4f,"
                  "ANG_X=%.4f,ANG_Y=%.4f,ANG_Z=%.4f,"
                  "TEMP_ACC=%d,TEMP_GYRO=%d,"
                  "RST_TMP=%u,RST_ZRO=%u"
                  "\r\n",
                  data->imu.accel.x, data->imu.accel.y, data->imu.accel.z,
                  data->imu.mag.x, data->imu.mag.y, data->imu.mag.z,
                  data->imu.gyro.x, data->imu.gyro.y, data->imu.gyro.z,
                  data->imu.quaternion.w, data->imu.quaternion.x, data->imu.quaternion.y, data->imu.quaternion.z,
                  data->imu.orientation.x, data->imu.orientation.y, data->imu.orientation.z,
                  data->imu.temp_accel, data->imu.temp_gyro,
                  data->imu.resets_temps, data->imu.resets_zeroes);

    if(temperature.getNumDevices() > 0) {
        logger.printf("TMP ");
        logger.printf("OW%d=%.4f", 0, data->temp[0].temp);
        for(int i = 1; i < temperature.getNumDevices(); i++) {
            logger.printf(",OW%d=%.4f", i, data->temp[i].temp);
        }
        logger.printf("\r\n");
    }

    logger.printf("SYS "
            "LOG_CNT=%hu,"
            "RTC=%u,"
            "LOCK=%u"
            "\r\n",
            data->system.log_cnt,
            data->system.rtc_s,
            data->system.lock_wait_us);
}
