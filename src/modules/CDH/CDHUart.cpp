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

    calculateChecksum(storage->packet);
    for(uint8_t i = 0; i < sizeof(CDHPacket); i++){
        cdh.putc(*(((uint8_t*)&packet)+i));
    }
}

void CDHUart::calculateChecksum(CDHPacket& data){
//    uint8_t* sData = (uint8_t*)(&data.data);
//    uint8_t checksum = 0x00;
//    for(uint8_t i=0; i < sizeof(SensorData); i++){
//        checksum ^= *(sData + i);
//    }

    uint32_t checksum = 0x00;

    __HAL_RCC_CRC_CLK_ENABLE();
    CRC_HandleTypeDef crc;
    crc.InputDataFormat = CRC_INPUTDATA_FORMAT_HALFWORDS;
    crc.Init = {
            DEFAULT_POLYNOMIAL_DISABLE,
            DEFAULT_INIT_VALUE_DISABLE,
            0x1021,
            CRC_POLYLENGTH_16B,
            checksum,
            CRC_OUTPUTDATA_INVERSION_DISABLE
    };
    if(HAL_CRC_Init(&crc) == HAL_OK){
        checksum = HAL_CRC_Calculate(&crc, reinterpret_cast<uint32_t*>(&data.data), sizeof(SensorData)/2);
    }else {
        printf("CRC INIT ERROR!");
    }
    HAL_CRC_DeInit(&crc);
    __HAL_RCC_CRC_CLK_DISABLE();
    printf("%lu\r\n",checksum);
    data.footer.checksum = (uint16_t)checksum;
}
