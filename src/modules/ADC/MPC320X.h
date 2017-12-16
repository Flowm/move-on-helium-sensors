/*
 * MPC320X.h
 *
 *  Created on: Dec 16, 2017
 *      Author: tkale
 */

#ifndef SRC_MODULES_ADC_MPC320X_H_
#define SRC_MODULES_ADC_MPC320X_H_

#include <mbed.h>

#define MPC_CHANNELS 8 // 0-7 : 8 channels on the MPC3208


/**
 * Driver for the MPC230X ADC's.
 * Configure the last digit of the device as the number of channels above.
 * WARNING: Only tested on the MPC2308!
 */
class MPC320X {
public:
    MPC320X(SPI *spi, DigitalOut *chipSelect);
    /**
     * Read a adcs value from a single channel.
     * @param channel [0-7] : What channel to read.
     * @return
     */
    uint16_t getValueSingle(uint8_t channel);
    /**
     * Read a difference between 2 consecutive channels
     * TODO
     * @param channel
     * @return
     */
    uint16_t getValueDifferential(uint8_t channel);
    /**
     * Tests all channels in single ended mode and prints the result on stdout.
     */
    void testAllChannels();
private:
    SPI *spi;
    DigitalOut *chipSelect;
};

#endif /* SRC_MODULES_ADC_MPC320X_H_ */
