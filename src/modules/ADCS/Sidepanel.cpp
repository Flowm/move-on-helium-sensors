#include "Sidepanel.hpp"

bool Sidepanel::setup() {
    return true;
}

void Sidepanel::update() {
    cs = 0;
    spi.write(NULL, 0, (char*) &data, sizeof(data));
    cs = 1;

    logger->lock();
    logger->printf("ADCS RAW ");
    for (int i=0; i<sizeof(data); i++) {
        logger->printf(" %02x", data.byte[i]);
    }
    logger->printf("\r\n");
    logger->unlock();
}
