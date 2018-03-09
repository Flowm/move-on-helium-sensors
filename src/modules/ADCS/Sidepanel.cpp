#include "Sidepanel.hpp"

bool Sidepanel::setup() {
    return true;
}

void Sidepanel::update() {
    cs = 0;
    spi.write(NULL, 0, (char*) &spi_data, sizeof(spi_data));
    cs = 1;

    logger->lock();
    logger->printf("ADCS RAW ");
    for (int i=0; i<sizeof(spi_data); i++) {
        logger->printf(" %02x", spi_data.byte[i]);
    }
    logger->printf("\r\n");
    logger->unlock();

    bool valid = checkVerifyData((uint8_t*) &spi_data.byte, &spi_data.data.end);
    if (valid) {
        logger->printf("ADCS Checksum valid\r\n");
        storage->lock();
        storage->unlock();
    }
}

bool Sidepanel::checkVerifyData(const uint8_t* data, VerifyStruct* verify) {
    // Basic check if data structure is valid
    if(verify->magic != 0xF0) {
#ifdef PRINT_CHECKSUM_ERRORS
        logger->printf("Verify: Invalid magic byte RECV:%02X EXP:%02X\r\n", verify->magic, 0xF0);
#endif
        return false;
    }

    // CRC-16-CCITT FALSE checksum to verify integrity
    uint16_t len = (uint8_t*)verify - data;
    uint16_t checksum = Checksum::crc16sw(data, len, 0xFFFF);
    if(verify->checksum != checksum) {
#ifdef PRINT_CHECKSUM_ERRORS
        logger->printf("Verify: Invalid checksum RECV:%04X EXP:%04X GEN:%02X\r\n", verify->checksum, checksum, verify->generation);
#endif
        return false;
    }

    return true;
}
