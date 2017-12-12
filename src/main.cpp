#include "mbed.h"

DigitalOut led(LED1);
Serial logger(A7, USBRX);
Serial cdh(D1, D0);
I2C gps(D4, D5);
I2C bno(D12, A6);
SPI spi(D2, A5, A1);
DigitalOut cs_bme680(D3);
DigitalOut cs_adc(D6);
DigitalOut ow_ds18b20(D9);

int main() {
    printf("\r\nRESET\r\n");

    while(1) {
        logger.printf("LOG\r\n");
        cdh.printf("CDH\r\n");

        led = !led;
        wait(1);
    }
}
