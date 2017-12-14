#include "Sensors.hpp"

void Sensors::setup() {
    printf("\r\nRESET\r\n");
}

void Sensors::loop() {
    while(1) {
        logger.printf("LOG\r\n");
        cdh.printf("CDH\r\n");
        Thread::wait(1000);
    }
}
