/*
 * MPC320X.cpp
 *
 *  Created on: Dec 16, 2017
 *      Author: tkale
 */

#include <ADC/MPC320X.h>

uint16_t MPC320X::getValueSingle(uint8_t channel) {

    // Reject Invalid channels
    if (channel >= MPC_CHANNELS) {
        return 0;
    }

    // Select the device by setting chip select low
    chipSelect = 0;
    // sending the 6 bits + 1 bit to ignore the null bit
    // coming from the device, so the data that is sent is 1100000
    // Padding - Start Bit(High)- SGL/DIFF - D2 - D1 - D0 - Sample Clock - Null(0)
    //    0               1           1      0    0    0       0              0
    // Datasheet page : 15; Table 5-2

    spi.write((0x18 | channel) << 2);
    printf("DEBUG: %d\r\n", (0x18 | channel) << 2);

    // now the device sends back the readings 12 bits, 8 bits at a time
    uint8_t high = spi.write(0x00);
    uint8_t low = spi.write(0x00);

    //printf("High : %d \r\n Low : %d \r\n", high , low);

    uint16_t result = ((uint16_t) high << 4) | ((uint16_t) low >> 4);

    // Deselect the device
    chipSelect = 1;

    return result;
}

void MPC320X::testAllChannels() {
    uint16_t channels[8];
    for (int i = 0; i <= 7; i++) {
        channels[i] = getValueSingle(i);
    }
    printf("ADC : ");
    for (int i = 0; i <= 7; i++) {
        printf(" %d ", channels[i]);
    }
    printf("\r\n");
}
