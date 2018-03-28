/*
 * GPS.hpp
 *
 *  Created on: Jan 21, 2018
 *      Author: tkale
 */

#pragma once

#include <mbed.h>
#include <time.h>
#include <lib/SensorThread/SensorThread.hpp>
#include <lib/minmea.git/minmea.h>

#define M8_ADDRESS  (0x42 << 1)
#define M8_DATALEN  0xFD // 2 bytes 0xFD,0xFE
#define M8_DATA     0xFF
// Reduce if running out of flash space. The longest buffer
// have seen is ~1500 chars long.
#define BUF_LEN     2000
#define NMEA_DELIM  "\r\n"

// UBX message to configure
// Dynamic Model = Airborne 2g mode.
// Min SV elevation  = 0;
// Fix Mode = 3D only

const char HA_MODE_CFG[] = {0xB5 ,0x62 ,0x06 ,0x24 ,0x24 ,0x00 ,0xFF ,0xFF ,0x07 ,0x02 ,0x00 ,0x00 ,
                            0x00 ,0x00 ,0x10 ,0x27 ,0x00 ,0x00 ,0x00 ,0x00 ,0xFA ,0x00 ,0xFA ,0x00 ,
                            0x64 ,0x00 ,0x5E ,0x01 ,0x00 ,0x3C ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,
                            0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x7F ,0x91};


class GPS : public SensorThread {
public:
    GPS(I2C* i2c, Storage* storage, SyncSerial* logger):
        SensorThread(storage, logger, "GPS"),
        i2c(i2c)
        {};

private:
    I2C* i2c;

    bool setup() override;

    /**
     * Processes the last buffer and schedules a new transaction.
     */
    void update() override;

    /**
     * Does a I2C read from an address.
     * Unused.
     */
    void readFromRegister(char device, char reg, char* data, uint32_t len, bool repeated);

    /**
     * Gets the length of the datastream from registers 0xFE and 0xFD.
     * #FIXME Does not work correctly!, always returns too high a value.
     * Currently not used.
     */
    uint16_t getDataLength();

    /**
     * Process the last read NMEA scentences.
     * Also prints the raw NMEA data to openlog.
     */
    void processBuffer();

    /**
     * Get the next chunk of NMEA scentences.
     */
    void getNextChunk(uint16_t len);

    /**
     * Callback which handles the result of the async I2C transaction.
     */
    void callback(int event);

    void print() override;

    char data[BUF_LEN + 1];
    bool t_flag = 0;

    minmea_sentence_gll gll;
    minmea_sentence_rmc rmc;
    minmea_sentence_vtg vtg;
    minmea_sentence_gga gga;

    SensorGPS gpsData;

};

