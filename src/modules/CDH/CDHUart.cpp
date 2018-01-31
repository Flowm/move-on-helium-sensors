/*
 * CDHUart.cpp
 *
 *  Created on: Dec 22, 2017
 *      Author: tkale
 */

#include <modules/CDH/CDHUart.hpp>


void CDHUart::setup(){
    cdh.baud(CDH_BAUD);
    cdh.attach(this, &CDHUart::rxCallback,MODSERIAL::RxIrq);
    queue_thread.start(callback(&queue, &EventQueue::dispatch_forever));
}


void CDHUart::rxCallback(MODSERIAL_IRQ_INFO* q){
    MODSERIAL* serial = q->serial;
    uint8_t command = serial->getc();
    queue.call(callback(this, &CDHUart::processCommand), command);
    return;
}

void CDHUart::processCommand(uint8_t command){
    storage->packet.header.status = command;
    transmitData();
//    printf("Command Processed : %d ", command);
}


void CDHUart::transmitData(){
    storage->lock();
    CDHPacket packet = storage->packet;
    storage->unlock();
    packet.footer.checksum = Checksum::crc16sw((uint8_t*)&packet.data, sizeof(SensorData));
    for(uint8_t i = 0; i < sizeof(CDHPacket); i++){
        cdh.putc(*(((uint8_t*)&packet)+i));
    }
}
