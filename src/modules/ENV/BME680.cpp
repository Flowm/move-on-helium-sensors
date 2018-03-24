#include "BME680.hpp"

bool BME680::setup() {
    bme.init(0);
    bme.reset();

    uint8_t chip_id = bme.getChipID();
    if (chip_id == _chip_id) {
        logger->lock();
        logger->printf("BME680 CHIP_ID: 0x%02x valid\r\n", chip_id);
        logger->unlock();
    } else {
        logger->lock();
        logger->printf("BME680 CHIP_ID: 0x%02x invalid\r\n", chip_id);
        logger->unlock();
        return false;
    }

    // Configure sensor
    bme.setOversampling(BME680_OVERSAMPLING_X4, BME680_OVERSAMPLING_X2, BME680_OVERSAMPLING_X16);
    bme.setIIRFilter(BME680_FILTER_3);
    bme.setGasOn(320, 150); // 320 degree Celsius and 150 milliseconds

    // Trigger a single THPG measurement
    bme.setForcedMode();

    // Increase thread priority to fix spi transmission errors caused by thread scheduling
    set_priority(osPriorityAboveNormal);
    set_update_rate(1000);

    return true;
}

void BME680::update() {
    ClosedCube_BME680_Status status = bme.readStatus();
    //printf("BME680 status: DF:%d, MF:%d, GF:%d, GI:%d\r\n", status.newDataFlag, status.measuringStatusFlag, status.gasMeasuringStatusFlag, status.gasMeasurementIndex);

    if (!status.newDataFlag) {
        if (_read_attempts++ > 1) {
            bme.setForcedMode();
            _read_attempts = 0;
        }
        last_data = 0;
        return;
    }

    data.temperature = bme.readTemperature();
    data.humidity = bme.readHumidity();
    data.pressure = bme.readPressure();
    data.gasresistance = bme.readGasResistance() / 1000.0;
    last_data = storage->get_ts();

    // Trigger a single THPG measurement
    bme.setForcedMode();

    // Store the data
    storage->lock();
    storage->data->env[_id] = data;
    storage->unlock();
}

void BME680::print() {
    // Only print valid data
    if (!last_data) {
        return;
    }

    logger->lock();
    logger->printf("%s%d T=%lu,"
                   "TEMP=%.2f,HUM=%.2f,PRES=%.2f,GAS=%.4f"
                   "\r\n",
                   _name, _id, last_data,
                   data.temperature,
                   data.humidity,
                   data.pressure,
                   data.gasresistance);
    logger->unlock();
}
