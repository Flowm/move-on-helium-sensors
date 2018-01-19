#include "BNO055.hpp"

void BNO055::setup() {
    getChipId();
}

void BNO055::update() {
    getChipId();
}

void BNO055::getChipId() {
    i2c.write(i2c_addr, &tx_buffer[0], 1);
    i2c.read(i2c_addr, &rx_buffer[0], 1);
    if (rx_buffer[0] == chip_id) {
        printf("BNO055 CHIP_ID: 0x%02x valid\r\n", rx_buffer[0]);
    } else {
        printf("BNO055 CHIP_ID: 0x%02x invalid\r\n", rx_buffer[0]);
    }

    memset(rx_buffer, 0, sizeof(rx_buffer));
}
