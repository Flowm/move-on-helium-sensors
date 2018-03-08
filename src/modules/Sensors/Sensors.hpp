#pragma once

#include <mbed.h>
#include <algorithm>
#include <include/pinmap.h>
#include <lib/MODSERIAL/MODSERIAL.h>
#include <lib/Storage/Storage.hpp>
#include <modules/ADC/MPC320X.hpp>
#include <modules/ENV/BME680.hpp>
#include <modules/IMU/BNO055IMU.hpp>
#include <modules/CDH/CDHUart.hpp>
#include <modules/Temperature/DS18B20.hpp>
#include <modules/GPS/GPS.hpp>
#include <lib/MODSERIAL/MODSERIAL.h>
#include <lib/SyncSerial/SyncSerial.hpp>

class Sensors {
public:
    Sensors() :
        // Interfaces
        led(LED1),
        logger(LOG_UART, USBRX),
        cdh(CDH_TX, CDH_RX),
        i2c_gps(GPS_SDA, GPS_SCL),
        i2c_imu(IMU_SDA, IMU_SCL),
        spi(SPI_MOSI, SPI_MISO, SPI_SCK),
        cs_adcs(SPI_CS_ADCS, 1),
        cs_env0(SPI_CS_ENV0, 1),
        cs_env1(SPI_CS_ENV1, 1),
        cs_sun(SPI_CS_SUN, 1),

        // CDH data
        cdhuart(cdh, &storage),

        // Sensors
        imu(i2c_imu, IMU_RST, &storage),
        env0(spi, cs_env0, &storage, 0),
        env1(spi, cs_env1, &storage, 1),
        env2(spi, cs_adcs, &storage, 2),

        temperature(TEMP_OW, &storage),
        gps(&i2c_gps, &storage, &logger)
        {};

    void setup();
    void loop();
    void log(uint16_t log_ms);

private:
    //Interfaces
    DigitalOut led;
    SyncSerial logger;
    MODSERIAL cdh;
    I2C i2c_gps;
    I2C i2c_imu;
    SPI spi;
    DigitalOut cs_adcs;
    DigitalOut cs_env0;
    DigitalOut cs_env1;
    DigitalOut cs_sun;

    // CDH data
    Storage storage;
    CDHUart cdhuart;

    // Sensors
    BNO055IMU imu;
    BME680 env0;
    BME680 env1;
    BME680 env2;
    DS18B20 temperature;
    GPS gps;

};
