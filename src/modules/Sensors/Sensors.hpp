#pragma once

#include <mbed.h>
#include <modules/ADC/MPC320X.h>
#include <modules/ENV/BME680.hpp>
#include <modules/IMU/BNO055.hpp>
#include <modules/CDH/CDHUart.hpp>
#include <MODSERIAL/MODSERIAL.h>

class Sensors {
public:
    Sensors() :
        // Interfaces
        led(LED1),
        logger(A7, USBRX),
        cdh(D1, D0),
        gps(D4, D5),
        bno(D12, A6),
        spi(D2, A5, A1),
        cs_bme680(D3, 1),
        cs_adc(D6, 1),
        ow_ds18b20(D9),
        // Sensors
        imu(bno),
        env(spi, cs_bme680),
        adc(spi, cs_adc),
        cdhuart(cdh)
        { setup(); };

    void setup();
    void loop();

private:
    //Interfaces
    DigitalOut led;
    Serial logger;
    MODSERIAL cdh;
    I2C gps;
    I2C bno;
    SPI spi;
    DigitalOut cs_bme680;
    DigitalOut cs_adc;
    DigitalOut ow_ds18b20;

    // Sensors
    BNO055 imu;
    BME680 env;
    MPC320X adc;

    // CDH data Handler
    CDHUart cdhuart;

};
