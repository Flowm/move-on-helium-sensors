/*
 * GPS.cpp
 *
 *  Created on: Jan 21, 2018
 *      Author: tkale
 */

#include <GPS/GPS.hpp>
#define BUF_LEN 100
void GPS::setup(){

//    i2c->frequency(200000);

    char data[BUF_LEN + 1];
//    char data1[BUF_LEN + 1];
    while(true){
        readFromRegister(M8_ADDRESS, M8_DATALEN, data, 2, false);
        uint16_t len = (uint16_t)data[0]<<8 | (uint16_t)data[1];
        printf("\r\nLen: %d:%d:%d\r\n", len, data[1], data[0]);
        while(len > 0 && len != 0xFFFF){
            readFromRegister(M8_ADDRESS, M8_DATALEN, data, (len > BUF_LEN)?BUF_LEN:len, false);
            /* Rudimentary NMEA Parsing:
            * Look for new lines
            * Look at the next ($GPGGA).len characters.
            * If match(){
            *   parse lat, lon time (May be invalid, only commas.!)
            *   Store in global data structure
            * }
            * Save other GPS data anyways.
            * Questions/Problems:
            *
            * How to save all GPS data!? --> Will not fit the sensor output format.
            * Carrier Phase info was wanted at some point.
            * What happens when no GPS fix
            * GPS update frequency --> 1 sec seems to work best since the chips puts out data
            * about once a second.
            *
            * Dont trust the Length field
            */
//            int i = 0 , j = 0;
//            while(i < 100) {
//                if((data[i] > 0x20 && data[i] < 0x7E) || (data[i] == 0x0D || data[i] == 0x0A)) {
//                    data1[j++] = data[i];
//                }
//                i++;
//            }
            data[(len > BUF_LEN)?BUF_LEN:len] = '\0';
            printf("%s",data);
            len -= (len > BUF_LEN)?BUF_LEN:len;
//            printf("\r\nLen1 : %d\r\n", len);
        }
        printf("\r\n");
        Thread::wait(1000);
    }

}

void GPS::readFromRegister(uint8_t device, uint8_t reg, char* data, uint8_t len, bool repeated){

    i2c->write(M8_ADDRESS, (char*)&reg, 1);
    i2c->read(M8_ADDRESS, data, len);
}

