#include "Sensors.hpp"

void Sensors::setup() {
    printf("\r\nRESET\r\n");
    imu.start();
    env.start();
    temperature.start();
}


void Sensors::loop() {

    while(1) {
        log();
        Thread::wait(1000);
    }
}

void Sensors::log() {
    storage.lock();
    SensorData* data = storage.data;
    logger.printf("D=ENV,"
                  "T=%4.2f,H=%4.2f,P=%6.2f,G=%6.3f"
                  "\r\n"
                  "D=IMU,"
                  "ACC_X=%4.2f,ACC_Y=%4.2f,ACC_Z=%4.2f,"
                  "MAG_X=%4.2f,MAG_Y=%4.2f,MAG_Z=%4.2f,"
                  "GYRO_X=%4.2f,GYRO_Y=%4.2f,GYRO_Z=%4.2f,"
                  "QUAT_W=%4.2f,QUAT_X=%4.2f,QUAT_Y=%4.2f,QUAT_Z=%4.2f,"
                  "ANG_X=%4.2f,ANG_Y=%4.2f,ANG_Z=%4.2f,"
                  "TEMP_ACC=%d,TEMP_GYRO=%d"
                  "\r\n",
                  data->env.temperature,
                  data->env.humidity,
                  data->env.pressure,
                  data->env.gasresistance,
                  data->imu.accel.x, data->imu.accel.y, data->imu.accel.z,
                  data->imu.mag.x, data->imu.mag.y, data->imu.mag.z,
                  data->imu.gyro.x, data->imu.gyro.y, data->imu.gyro.z,
                  data->imu.quaternion.w, data->imu.quaternion.x, data->imu.quaternion.y, data->imu.quaternion.z,
                  data->imu.orientation.x, data->imu.orientation.y, data->imu.orientation.z,
                  data->imu.temp_accel, data->imu.temp_gyro
                 );
    logger.printf("D= TMP ");
    for(int i = 0; i < temperature.getNumDevices(); i++) {
        logger.printf(", T%d = %f ", i, data->temp[i].temp);
    }
    logger.printf("\r\n");
    storage.unlock();
}
