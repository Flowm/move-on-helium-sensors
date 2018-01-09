/*
 * CDHUart.cpp
 *
 *  Created on: Dec 22, 2017
 *      Author: tkale
 */

#include <CDH/CDHUart.hpp>


void CDHUart::setup(){
    cdh.baud(CDH_BAUD);
    cdh.attach(this, &CDHUart::rxCallback,MODSERIAL::RxIrq);
}


void CDHUart::rxCallback(MODSERIAL_IRQ_INFO* q){
    MODSERIAL* serial = q->serial;
    uint8_t size = serial->rxBufferGetCount();
    size = (size > COMMAND_BUF_LEN)?COMMAND_BUF_LEN:size;
    serial->move((char*)opCodes, size);
    numCommands = size;
    //Writes to UART in an interrupt cause a lockup!
}


void CDHUart::transmitData(){
    if(numCommands > 0){
        for(int i = 0; i < numCommands; i++){
            data.header.status = opCodes[i];
            calculateChecksum(data);
            for(uint8_t i = 0; i < sizeof(CDHPacket); i++){
                cdh.putc(*(((uint8_t*)&data)+i));
            }
        }
        numCommands = 0;
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
