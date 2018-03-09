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
    for (unsigned int i=0; i<sizeof(spi_data); i++) {
        logger->printf(" %02x", spi_data.byte[i]);
    }
    logger->printf("\r\n");
    logger->unlock();

    bool valid = checkVerifyData((uint8_t*) &spi_data.byte, &spi_data.data.end);
    if (valid) {
        storage->lock();
        storage->data->adcs.accel.x = spi_data.data.sensors.acc.x;
        storage->data->adcs.accel.y = spi_data.data.sensors.acc.y;
        storage->data->adcs.accel.z = spi_data.data.sensors.acc.z;
        storage->data->adcs.gyro.x = spi_data.data.sensors.gyro.x;
        storage->data->adcs.gyro.y = spi_data.data.sensors.gyro.y;
        storage->data->adcs.gyro.z = spi_data.data.sensors.gyro.z;
        storage->data->adcs.mag.x = spi_data.data.sensors.mag.x;
        storage->data->adcs.mag.y = spi_data.data.sensors.mag.y;
        storage->data->adcs.mag.z = spi_data.data.sensors.mag.z;
        storage->data->adcs.raw_mag[0] = spi_data.data.sensors.rawMag.x;
        storage->data->adcs.raw_mag[1] = spi_data.data.sensors.rawMag.y;
        storage->data->adcs.raw_mag[2] = spi_data.data.sensors.rawMag.z;
        storage->data->adcs.raw_sun[0] = spi_data.data.sensors.rawSun.pads[0];
        storage->data->adcs.raw_sun[1] = spi_data.data.sensors.rawSun.pads[1];
        storage->data->adcs.raw_sun[2] = spi_data.data.sensors.rawSun.pads[2];
        storage->data->adcs.raw_sun[3] = spi_data.data.sensors.rawSun.pads[3];
        storage->unlock();
    }
}

bool Sidepanel::checkVerifyData(const uint8_t* data, VerifyStruct* verify) {
    // Basic check if data structure is valid
    if(verify->magic != 0xF0) {
#ifdef PRINT_CHECKSUM_ERRORS
        logger->printf("ADCS Invalid magic byte RECV:%02X EXP:%02X\r\n", verify->magic, 0xF0);
#endif
        return false;
    }

    // CRC-16-CCITT FALSE checksum to verify integrity
    uint16_t len = (uint8_t*)verify - data;
    uint16_t checksum = Checksum::crc16sw(data, len, 0xFFFF);
    if(verify->checksum != checksum) {
#ifdef PRINT_CHECKSUM_ERRORS
        logger->printf("ADCS Invalid checksum RECV:%04X EXP:%04X GEN:%02X\r\n", verify->checksum, checksum, verify->generation);
#endif
        return false;
    }

    return true;
}
