/*
 * CDHUart.cpp
 *
 *  Created on: Dec 22, 2017
 *      Author: tkale
 */

#include <CDH/CDHUart.hpp>


void CDHUart::setup(){
    cdh.baud(CDH_BAUD);
    cdh.rxBufferSetSize(COMMAND_BUF_LEN);
    cdh.attach(this, &CDHUart::rxCallback,MODSERIAL::RxIrq);
    queue_thread.start(callback(&queue, &EventQueue::dispatch_forever));
}


void CDHUart::rxCallback(MODSERIAL_IRQ_INFO* q){
    MODSERIAL* serial = q->serial;
    uint8_t command = serial->rxGetLastChar();
    queue.call(callback(this, &CDHUart::processCommand), command);
    return;
}

void CDHUart::processCommand(uint8_t command){
    data.header.status = command;
    transmitData();
//    printf("Command Processed : %d ", command);
}


void CDHUart::transmitData(){
    calculateChecksum(data);
    for(uint8_t i = 0; i < sizeof(CDHPacket); i++){
        cdh.putc(*(((uint8_t*)&data)+i));
    }
}

void CDHUart::calculateChecksum(CDHPacket& data){
    uint8_t* sData = (uint8_t*)(&data.data);
    uint8_t checksum = 0x00;
    for(uint8_t i=0; i < sizeof(SensorData); i++){
        checksum ^= *(sData + i);
    }
    data.footer.checksum = checksum;
}
