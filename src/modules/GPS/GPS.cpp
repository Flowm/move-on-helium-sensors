/*
 * GPS.cpp
 *
 *  Created on: Jan 21, 2018
 *      Author: tkale
 */

#include <GPS/GPS.hpp>

void GPS::setup(){
//	i2c.frequency(100000)
//	i2c.read()
//	char data[100];
//	readFromAddress(0xFD,)
	char data[100];
	int ack = i2c.read(M8_ADDRESS,data, false);
	printf("GPS ACK: %d\r\n",ack);

	readFromRegister(M8_ADDRESS, M8_DATALEN, data, 2, false);

}

void GPS::readFromRegister(uint8_t address, uint8_t reg, char* data, uint8_t len, bool repeated){

	int ack = 0;
	ack = i2c.write(address);
	printf("ADDRESS ACK: %d\r\n",ack);
	ack = i2c.write(reg);
	printf("REG ADDRESS ACK: %d\r\n",ack);
	ack = i2c.read(address, data, len, repeated);
	printf("%d : %d",data[0],data[1]);
}

