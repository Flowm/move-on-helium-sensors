#include "Sidepanel.hpp"

bool Sidepanel::setup() {
    // Set sidepanel to NORMAL mode to enable the BMX055 sensor
    control.mode.mode = SidepanelMode::NORMAL;
    control.mode.generation++;
    updateVerifyData((uint8_t*) &control, &control.end);

    //Set update rate of sidepanel thread
    set_update_rate(100);
    return true;
}

void Sidepanel::update() {
    spi->lock();
    *cs = 0;
    spi->write((char*) &control, sizeof(SidepanelControl), (char*) recv, sizeof(recv));
    *cs = 1;
    spi->unlock();

#ifdef SIDEPANEL_RAW_DATA
    logger->lock();
    logger->printf("ADCS RAW ");
    for (unsigned int i = 0; i < sizeof(SidepanelData); i++) {
        logger->printf(" %02x", ((char*) data)[i]);
    }
    logger->printf("\r\n");
    logger->unlock();
#endif

    bool valid = checkVerifyData((uint8_t*) data, &data->end);
    if (valid) {
        storage->lock();
        //storage->data->adcs.accel.x = data->sensors.acc.x;
        //storage->data->adcs.accel.y = data->sensors.acc.y;
        //storage->data->adcs.accel.z = data->sensors.acc.z;
        storage->data->adcs.gyro.x = data->sensors.gyro.x;
        storage->data->adcs.gyro.y = data->sensors.gyro.y;
        storage->data->adcs.gyro.z = data->sensors.gyro.z;
        storage->data->adcs.mag.x = data->sensors.mag.x * data->sensors.mag.length;
        storage->data->adcs.mag.y = data->sensors.mag.y * data->sensors.mag.length;
        storage->data->adcs.mag.z = data->sensors.mag.z * data->sensors.mag.length;
        storage->data->adcs.sun.x = data->sensors.sun.x;
        storage->data->adcs.sun.y = data->sensors.sun.y;
        storage->data->adcs.sun.z = data->sensors.sun.z;
        //storage->data->adcs.tbmx = data->sensors.temp.bmx / 2.0 + 23;
        storage->data->adcs.temp[0] = (int16_t) data->sensors.temp.t1 / 16.0;
        storage->data->adcs.temp[1] = (int16_t) data->sensors.temp.t2 / 16.0;
        storage->data->adcs.temp[2] = (int16_t) data->sensors.temp.t3 / 16.0;
        //storage->data->adcs.raw_mag[0] = data->sensors.rawMag.x;
        //storage->data->adcs.raw_mag[1] = data->sensors.rawMag.y;
        //storage->data->adcs.raw_mag[2] = data->sensors.rawMag.z;
        storage->data->adcs.raw_sun[0] = data->sensors.rawSun.pads[0];
        storage->data->adcs.raw_sun[1] = data->sensors.rawSun.pads[1];
        storage->data->adcs.raw_sun[2] = data->sensors.rawSun.pads[2];
        storage->data->adcs.raw_sun[3] = data->sensors.rawSun.pads[3];
        storage->unlock();
    }
}

void Sidepanel::updateVerifyData(const uint8_t* data, VerifyStruct* verify) {
    // Set valid end data (Only really required once)
    verify->magic = 0xF0;

    // Calculate checksum
    uint16_t len = (uint8_t*)verify - data;
    verify->checksum = Checksum::crc16sw(data, len, 0xFFFF);
}

bool Sidepanel::checkVerifyData(const uint8_t* data, VerifyStruct* verify) {
    // Basic check if data structure is valid
    if(verify->magic != 0xF0) {
#ifdef SIDEPANEL_PRINT_CHECKSUM_ERRORS
        logger->lock();
        logger->printf("ADCS Invalid magic byte RECV:%02X EXP:%02X\r\n", verify->magic, 0xF0);
        logger->unlock();
#endif
        return false;
    }

    // CRC-16-CCITT FALSE checksum to verify integrity
    uint16_t len = (uint8_t*)verify - data;
    uint16_t checksum = Checksum::crc16sw(data, len, 0xFFFF);
    if(verify->checksum != checksum) {
#ifdef SIDEPANEL_PRINT_CHECKSUM_ERRORS
        logger->lock();
        logger->printf("ADCS Invalid checksum RECV:%04X EXP:%04X GEN:%02X\r\n", verify->checksum, checksum, verify->generation);
        logger->unlock();
#endif
        return false;
    }

    return true;
}

void Sidepanel::print() {
    logger->lock();
    logger->printf("ADCS "
                   "GYRO_X=%.4f,GYRO_Y=%.4f,GYRO_Z=%.4f,"
                   "MAG_X=%.4f,MAG_Y=%.4f,MAG_Z=%.4f,"
                   "SUN_X=%.4f,SUN_Y=%.4f,SUN_Z=%.4f,"
                   "TEMP_OW1=%.4f,TEMP_OW2=%.4f,TEMP_OW3=%.4f,"
                   "SUN_RAW1=%hu,SUN_RAW2=%hu,SUN_RAW3=%hu,SUN_RAW4=%hu"
                   "\r\n",
                   //data->adcs.accel.x, data->adcs.accel.y, data->adcs.accel.z,
                   data->sensors.gyro.x, data->sensors.gyro.y, data->sensors.gyro.z,
                   data->sensors.mag.x, data->sensors.mag.y, data->sensors.mag.z,
                   data->sensors.sun.x, data->sensors.sun.y, data->sensors.sun.z,
                   (int16_t) data->sensors.temp.t1 / 16.0, (int16_t) data->sensors.temp.t2 / 16.0, (int16_t) data->sensors.temp.t3 / 16.0,
                   data->sensors.rawSun.pads[0], data->sensors.rawSun.pads[1], data->sensors.rawSun.pads[2], data->sensors.rawSun.pads[3]);
    logger->unlock();
}
