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

Thread thread;
void led_thread() {
    while (true) {
        led = !led;
        Thread::wait(1000);
    }
}

char tx_buffer[8] = {0};
char rx_buffer[8] = {0};
const int bno055_addr = 0x28 << 1;

int main() {
    printf("\r\nRESET\r\n");
    thread.start(led_thread);
    cs_bme680 = 1;
    cs_adc = 1;

    while(1) {
        logger.printf("LOG\r\n");
        cdh.printf("CDH\r\n");

        tx_buffer[0] = 0x0;
        bno.write(bno055_addr, &tx_buffer[0], 1);
        bno.read(bno055_addr, &rx_buffer[0], 1);
        printf("BNO055 CHIP_ID: 0x%02x\r\n", rx_buffer[0]);
        memset(rx_buffer, 0, sizeof(rx_buffer));

        cs_bme680 = 0;
        tx_buffer[0] = 0x50 | 0x80;
        spi.write(&tx_buffer[0], 1, &rx_buffer[0], 2);
        printf("BME680 CHIP_ID: 0x%02x\r\n", rx_buffer[1]);
        cs_bme680 = 1;

        memset(rx_buffer, 0, sizeof(rx_buffer));
        Thread::wait(1000);
    }
}
