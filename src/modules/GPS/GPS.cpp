/*
 * GPS.cpp
 *
 *  Created on: Jan 21, 2018
 *      Author: tkale
 */

#include <GPS/GPS.hpp>

bool GPS::setup(){

    i2c->frequency(400000);
    set_update_rate(1000);

    return true;
}



void GPS::update() {

    if(t_flag){

//        uint16_t len  = getDataLength();
//        if(len != 0xFF) {
            t_flag = false;
            processBuffer();
            getNextChunk(0);
//        }

//        Thread::wait(1000);
    }
}

void GPS::processBuffer() {
    // Sanitize buffer.
    printf("\r\nProcessing\r\n");
//    for(int i = 0; i < BUF_LEN; i++) {
//        if(data[i] == 0xFF) {
//            data[i] = '\0';
//            break;
//        }
//    }
    char *scentence = strtok(data,NMEA_DELIM);
    while(scentence != NULL) {

        // 0xFF is the default value returned when no more
        // data is present.
        if(scentence[0] == 0xFF) break;


        //#TODO What prefix will not break influx?
        fputs("RAWGPS ", stdout);
        puts(scentence);

        // Parse the scentence.
        switch(minmea_sentence_id(scentence,false)) {
        case MINMEA_SENTENCE_GLL:
            minmea_parse_gll(&gll, scentence);
            break;
        case MINMEA_SENTENCE_RMC:
            minmea_parse_rmc(&rmc, scentence);
            break;
        default:
            // #TODO: Parse more scentence types here.
            break;
        }
        scentence = strtok(NULL, NMEA_DELIM);
    }

    SensorGPS gpsData;
    timespec ts;

    if(rmc.valid){
        gpsData.latitude = minmea_tocoord(&rmc.latitude);
        gpsData.longitude = minmea_tocoord(&rmc.longitude);
        gpsData.groundSpeed = minmea_tofloat(&rmc.speed);
        gpsData.course = minmea_tofloat(&rmc.course);
        gpsData.magVar = minmea_tofloat(&rmc.variation);

        minmea_gettime(&ts, &rmc.date, &rmc.time);
        gpsData.timestamp = ts.tv_sec;
    }

    storage->lock();
    storage->data->gps = gpsData;
    storage->unlock();

}

void GPS::getNextChunk(uint16_t len){
    char reg[2] = {M8_DATA};
    i2c->transfer(M8_ADDRESS, reg, 2, (char*)data, BUF_LEN, event_callback_t(this, &GPS::callback), I2C_EVENT_ALL);
}



void GPS::readFromRegister(char device, char reg, char* data, uint32_t len, bool repeated = false) {
    i2c->write(M8_ADDRESS, &reg, repeated);
    i2c->read(M8_ADDRESS, data, len);
}

uint16_t GPS::getDataLength() {

    char len[2];
    readFromRegister(M8_ADDRESS, M8_DATALEN, len, 2, false);

    uint16_t len16 = (((uint16_t)len[0] << 8) | (uint16_t)len[1]);
    printf("Len:%d\r\n",len16);
    return len16;
}

void GPS::callback(int event){
    t_flag = true;
}

