#include "BME680.hpp"

bool BME680::setup() {
    bme.init(0);
    bme.reset();

    uint8_t chip_id = bme.getChipID();
    if (chip_id == _chip_id) {
        printf("BME680 CHIP_ID: 0x%02x valid\r\n", chip_id);
    } else {
        printf("BME680 CHIP_ID: 0x%02x invalid\r\n", chip_id);
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
        return;
    }

    _temperature = bme.readTemperature();
    _humidity = bme.readHumidity();
    _pressure = bme.readPressure();
    _gasresistance = bme.readGasResistance() / 1000.0;
    //printf("BME680 %4.2f *C; %4.2f Pa; %6.2f %%; %6.3f kOhm\r\n", _temperature, _humidity, _pressure, _gasresistance);

    // Trigger a single THPG measurement
    bme.setForcedMode();

    // Store the data
    storage->lock();
    storage->data->env[id].temperature = _temperature;
    storage->data->env[id].humidity = _humidity;
    storage->data->env[id].pressure = _pressure;
    storage->data->env[id].gasresistance = _gasresistance;
    storage->unlock();
}
