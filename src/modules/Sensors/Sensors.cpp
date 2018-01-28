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
    logger.printf("D=ENV,T=%4.2f,H=%4.2f;P=%6.2f;G=%6.3f "
                  "ACCEL x=%4.2f, y=%4.2f, z=%4.2f "
                  "MAG x=%4.2f, y=%4.2f, z=%4.2f "
                  "GYRO x=%4.2f, y=%4.2f, z=%4.2f "
                  "QUAT w=%4.2f, x=%4.2f, y=%4.2f, z=%4.2f "
                  "ANGLES h=%4.2f, r=%4.2f, p=%4.2f "
                  "\r\n",
                  data->env.temperature,
                  data->env.humidity,
                  data->env.pressure,
                  data->env.gasresistance,
                  data->imu.accel.x, data->imu.accel.y, data->imu.accel.z,
                  data->imu.mag.x, data->imu.mag.y, data->imu.mag.z,
                  data->imu.gyro.x, data->imu.gyro.y, data->imu.gyro.z,
                  data->imu.quaternion.r, data->imu.quaternion.i, data->imu.quaternion.j, data->imu.quaternion.k,
                  data->imu.orientation.h, data->imu.orientation.r, data->imu.orientation.p
                 );
    storage.unlock();
}
