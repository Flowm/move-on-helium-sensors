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
    logger.printf("D=ENV,T=%4.2f,H=%4.2f;P=%6.2f;G=%6.3f\r\n",
                  data->env.temperature,
                  data->env.humidity,
                  data->env.pressure,
                  data->env.gasresistance
                 );
    storage.unlock();
}
