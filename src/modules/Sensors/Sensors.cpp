#include "Sensors.hpp"

void Sensors::setup() {
    printf("\r\nRESET\r\n");
    imu.setup();
    env.setup();
}


void Sensors::loop() {

    while(1) {
       logger.printf("LOG1\r\n");
       Thread::wait(1000);
    }
}
