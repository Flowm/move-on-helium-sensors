#pragma once

#include <mbed.h>
#include <modules/ADC/MPC320X.h>

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
        cs_bme680(D3),
        cs_adc(D6),
        ow_ds18b20(D9),
        // Sensors
        adc(&spi, &cs_adc)
        { setup(); };

    void setup();
    void loop();

private:
    //Interfaces
    DigitalOut led;
    Serial logger;
    Serial cdh;
    I2C gps;
    I2C bno;
    SPI spi;
    DigitalOut cs_bme680;
    DigitalOut cs_adc;
    DigitalOut ow_ds18b20;

    // Sensors
    MPC320X adc;
};
