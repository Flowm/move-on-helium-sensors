#pragma once

#include <mbed.h>
#include <include/pinmap.h>
#include <lib/MODSERIAL/MODSERIAL.h>
#include <lib/Storage/Storage.hpp>
#include <modules/ADC/MPC320X.hpp>
#include <modules/ENV/BME680.hpp>
#include <modules/IMU/BNO055IMU.hpp>
#include <modules/CDH/CDHUart.hpp>
#include <modules/Temperature/DS18B20.hpp>


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
        cs_eno(SPI_CS_ADCS, 1),
        cs_env(SPI_CS_ENV, 1),
        cs_sun(SPI_CS_ADCS, 1),

        // CDH data
        cdhuart(cdh, &storage),

        // Sensors
        imu(i2c_imu, &storage),
        env(spi, cs_env, &storage),
        temperature(TEMP_OW, &storage)
        {};

    void setup();
    void loop();
    void log();

private:
    //Interfaces
    DigitalOut led;
    Serial logger;
    MODSERIAL cdh;
    I2C i2c_gps;
    I2C i2c_imu;
    SPI spi;
    DigitalOut cs_adcs;
    DigitalOut cs_eno;
    DigitalOut cs_env;
    DigitalOut cs_sun;

    // CDH data
    Storage storage;
    CDHUart cdhuart;

    // Sensors
    BNO055IMU imu;
    BME680 env;
    DS18B20 temperature;

};
