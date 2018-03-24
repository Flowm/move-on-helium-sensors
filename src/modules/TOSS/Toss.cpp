#include "Toss.hpp"

bool Toss::setup() {
    set_update_rate(250);

    return true;
}

void Toss::update() {
    spi->lock();
    *cs = 0;
    spi->write(NULL, 0, recv, sizeof(recv));
    *cs = 1;
    spi->unlock();

#ifdef TOSS_RAW_DATA
    logger->lock();
    logger->printf("TOSS RAW ");
    for (unsigned int i = 0; i < sizeof(recv); i++) {
        logger->printf(" %02x", ((char*) data)[i]);
    }
    logger->printf("\r\n");
    logger->unlock();
#endif

    uint16_t len = (uint8_t*)&data->checksum - (uint8_t*)data;
    uint16_t checksum = Checksum::crc16sw((uint8_t*)data, len);
    if (data->checksum == checksum) {
        last_data = storage->get_ts();
        storage->lock();
        storage->data->toss = *data;
        storage->unlock();
    } else {
        last_data = 0;
        logger->lock();
        logger->printf("TOSS Invalid checksum RECV:%04X EXP:%04X\r\n", data->checksum, checksum);
        logger->unlock();
    }
}

void Toss::print() {
    // Only print valid data
    if (!last_data) {
        return;
    }

    logger->lock();
    logger->printf("%s T=%lu,"
                   "RTC=%u",
                   _name, last_data,
                   data->timestamp);
    for (int i = 0; i < 4; i++) {
        logger->printf(",TMP%u=%d", i+1, data->temp[i]);
    }
    for (int i = 0; i < 9; i++) {
        logger->printf(",PH%u=%d", i+1, data->photodiode[i]);
    }
    logger->printf("\r\n");
    logger->unlock();
}
