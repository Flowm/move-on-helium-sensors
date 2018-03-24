/*
 * GPS.cpp
 *
 *  Created on: Jan 21, 2018
 *      Author: tkale
 */

#include <GPS/GPS.hpp>

bool GPS::setup() {
    i2c->frequency(400000);
    set_update_rate(1000); // 2000 doesnot work.

    // Write the config message and wait 100 ms to allow for configuration.
    // #TODO: Parse the UBX-ACK-ACK to really verify that its acknowledged.
    for(int i=0; i < 3; i++) {
        i2c->write(M8_ADDRESS, HA_MODE_CFG, sizeof(HA_MODE_CFG), false);
        Thread::wait(200);
    }

    return true;
}

void GPS::update() {

    if(t_flag){
        t_flag = false;
//        uint16_t len = getDataLength();
        processBuffer();
        getNextChunk(0);
    }
}

void GPS::processBuffer() {

    char *sentence = strtok(data,NMEA_DELIM);
    while(sentence != NULL) {

        // 0xFF is the default value returned when no more
        // data is present.
        if(sentence[0] == 0xFF) break;

        logger->lock();
        logger->puts("RAWGPS ");
        logger->puts(sentence);
        logger->puts("\r\n");
        logger->unlock();

        // Parse the scentence.
        switch(minmea_sentence_id(sentence,false)) {
        case MINMEA_SENTENCE_GLL:
            minmea_parse_gll(&gll, sentence);
            break;
        case MINMEA_SENTENCE_RMC:
            minmea_parse_rmc(&rmc, sentence);
            break;
        case MINMEA_SENTENCE_VTG:
            minmea_parse_vtg(&vtg, sentence);
            break;
        case MINMEA_SENTENCE_GGA:
            minmea_parse_gga(&gga, sentence);
            break;
        default:
            // #TODO: Parse more scentence types here.
            break;
        }
        sentence = strtok(NULL, NMEA_DELIM);
    }


    if(gga.fix_quality != 0){
        gpsData.lat = minmea_tocoord(&gga.latitude);
        gpsData.lon = minmea_tocoord(&gga.longitude);
        gpsData.altitude = minmea_tofloat(&gga.altitude);
        gpsData.trueTrack = minmea_tofloat(&vtg.true_track_degrees);
        gpsData.groundSpeed = minmea_tofloat(&vtg.speed_kph);
        last_data = storage->get_ts();
    } else {
        last_data = 0;
    }

    timespec ts;
    if(!minmea_gettime(&ts, &rmc.date, &gga.time)) {
        gpsData.timestamp = ts.tv_sec;
    }

    storage->lock();
    storage->data->gps = gpsData;
    storage->unlock();
}

void GPS::getNextChunk(uint16_t len) {
    // XXX: The read address of M8_DATA works, but I dont know why!.
    //      Changing it still works.
    //      It seems that the write is being ignored. So writing nothing works
    //      because the I2C register starts at 0xFF by default.
    i2c->transfer(M8_ADDRESS, NULL, 0, (char*)data, BUF_LEN, event_callback_t(this, &GPS::callback), I2C_EVENT_ALL);
}

void GPS::readFromRegister(char device, char reg, char* data, uint32_t len, bool repeated = false) {
    i2c->write(M8_ADDRESS, &reg, repeated);
    i2c->read(M8_ADDRESS, data, len);
}

uint16_t GPS::getDataLength() {
    char len[2];
    readFromRegister(M8_ADDRESS, M8_DATALEN, len, 2, false);

    uint16_t len16 = (((uint16_t)len[0] << 8) | (uint16_t)len[1]);
    //printf("Len:%d\r\n",len16);
    return len16;
}

void GPS::callback(int event) {
    t_flag = true;
}

void GPS::print() {
    // Only print valid data
    if (!last_data) {
        return;
    }

    logger->lock();
    logger->printf("%s T=%lu,"
                   "LAT=%.6f,LON=%.6f,ALT=%.4f,"
                   "TIME=%u,SPEED=%.4f,TRUETRK=%.4f"
                   "\r\n",
                   _name, last_data,
                   gpsData.lat, gpsData.lon, gpsData.altitude,
                   gpsData.timestamp, gpsData.groundSpeed, gpsData.trueTrack);
    logger->unlock();
}

