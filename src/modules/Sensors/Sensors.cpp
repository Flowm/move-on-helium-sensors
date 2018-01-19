#include "Sensors.hpp"

void Sensors::setup() {
    printf("\r\nRESET\r\n");
    imu.start();
    env.start();
}

void Sensors::loop() {
    while(1) {
        logger.printf("LOG1\r\n");
        Thread::wait(5000);
    }
}
