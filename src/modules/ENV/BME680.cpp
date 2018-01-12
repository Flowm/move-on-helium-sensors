#include "BME680.hpp"

void BME680::setup() {
    cs = 0;
    tx_buffer[0] = 0x50 | 0x80;
    spi.write(&tx_buffer[0], 1, &rx_buffer[0], 2);
    if (rx_buffer[1] == chip_id) {
        printf("BME680 CHIP_ID: 0x%02x valid\r\n", rx_buffer[1]);
    } else {
        printf("BME680 CHIP_ID: 0x%02x invalid\r\n", rx_buffer[1]);
    }
    cs = 1;

    memset(rx_buffer, 0, sizeof(rx_buffer));
}

void BME680::read() {
}
