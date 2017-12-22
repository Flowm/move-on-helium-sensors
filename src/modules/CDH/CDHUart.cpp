/*
 * CDHUart.cpp
 *
 *  Created on: Dec 22, 2017
 *      Author: tkale
 */

#include <CDH/CDHUart.hpp>


void CDHUart::setup(){
    cdh.baud(115200);
}

void write_callback(int){
    int a = 0;
    printf("ok:%d\r\n", a);
}

void CDHUart::transmitData(){
//    cdh.printf("asd\r\n");
    CDHPacket data = {  {0xCA,0xFE,sizeof(data)},
                        {'a','b','c'},
                        {0xFE,0xCA}};

    printf("data:%d\r\n",sizeof(CDHPacket));
//
////       Callback<void()> cb = &write_callback;
////       const event_callback_t c;
////       c.attach(*write_callback)
////////       c.attach(&write_callback);
//////
////       cdh.write((uint8_t*)data, sizeof(data), void,Serial::TxIrq);
////////       cdh.write()
////
//////       cdh.write((uint8_t*)data,sizeof(data), callback(&Sensors::cdhWriteCallback , &Sensors));
/////
//    const event_callback_t call;
////       call.attach(this, &Sensors::cdhWriteCallback);
        cdh.write((uint8_t*)&data, sizeof(CDHPacket), &write_callback, Serial::TxIrq);
//
//       for(int i=0;i <10 ; i++){
//           for(int j=0; j<sizeof(CDHPacket); j++ ){
//               cdh.putc(*(binary+j));
//           }
//           data->data.gyro[0]++;
           cdh.printf("\r\n");
//
//       }
}

