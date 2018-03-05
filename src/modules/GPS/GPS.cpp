/*
 * GPS.cpp
 *
 *  Created on: Jan 21, 2018
 *      Author: tkale
 */

#include <GPS/GPS.hpp>

bool GPS::setup(){

    i2c->frequency(400000);
    set_update_rate(2000);

    return true;
}


/*
 * Update function.
 * Processes the last buffer and schedules a new transaction.
 */
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
    // Process the buffer, parse scentences and output to logger;
    printf("\r\nProcessing\r\n");
    for(int i = 0; i < BUF_LEN; i++) {
        if(data[i] == 0xFF) {
            data[i] = '\0';
            break;
        }
    }
    char *scentence = strtok(data,"\r\n");
    while(scentence != NULL) {
        puts(scentence);
        switch(minmea_sentence_id(scentence,false)) {
        case MINMEA_SENTENCE_GLL:
            minmea_parse_gll(&gll, scentence);
            break;
        case MINMEA_SENTENCE_RMC:
            minmea_parse_rmc(&rmc, scentence);
            break;
        }
        scentence = strtok(NULL, "\r\n");
    }
    timespec ts;
    minmea_gettime(&ts, &rmc.date, &rmc.time);

    storage->lock();
    storage->data->gps.latitude = minmea_tocoord(&rmc.latitude);
    storage->data->gps.longitude = minmea_tocoord(&rmc.longitude);
    storage->data->gps.sec = ts.tv_sec;
    storage->data->gps.nsec = ts.tv_nsec;
    storage->unlock();
    printf("Coodrinates:%f N, %f E \r\n", minmea_tocoord(&gll.latitude), minmea_tocoord(&gll.longitude));
    printf("Time: %d s , %d nsec\r\n" , ts.tv_sec, ts.tv_nsec);
}

/*
 * Schedules the next transfer on I2C.
 * The data returned will be processed on the next update loop.
 *
 * #TODO: Implement GPS on its own independent thread to have a
 * better response time.
 */
void GPS::getNextChunk(uint16_t len){
    char reg[2] = {M8_DATA};
    i2c->transfer(M8_ADDRESS, reg, 2, (char*)data, BUF_LEN, event_callback_t(this, &GPS::callback), I2C_EVENT_ALL);
}



void GPS::readFromRegister(uint16_t device, uint16_t reg, char* data, uint32_t len, bool repeated = false) {
    i2c->write(M8_ADDRESS, (char*)&reg, repeated);
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

