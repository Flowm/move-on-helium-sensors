#include "Sensors.hpp"

void Sensors::setup() {
    printf("\r\nRESET\r\n");
    imu.start();
    env.start();
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
                  "QUAT_R=%4.2f,QUAT_I=%4.2f,QUAT_J=%4.2f,QUAT_K=%4.2f,"
                  "ANG_X=%4.2f,ANG_Y=%4.2f,ANG_Z=%4.2f"
                  "\r\n",
                  data->env.temperature,
                  data->env.humidity,
                  data->env.pressure,
                  data->env.gasresistance,
                  data->imu.accel.x, data->imu.accel.y, data->imu.accel.z,
                  data->imu.mag.x, data->imu.mag.y, data->imu.mag.z,
                  data->imu.gyro.x, data->imu.gyro.y, data->imu.gyro.z,
                  data->imu.quaternion.r, data->imu.quaternion.i, data->imu.quaternion.j, data->imu.quaternion.k,
                  data->imu.orientation.x, data->imu.orientation.y, data->imu.orientation.z
                 );
    storage.unlock();
}
