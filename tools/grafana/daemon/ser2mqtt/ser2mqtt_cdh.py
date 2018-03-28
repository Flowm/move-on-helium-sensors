#!/usr/bin/env python3

import logging
import serial
from lib.input.serial_wrap import SerialWrap
from lib.parse.cdh_bin_parse import CDHBinParserExtended 
from lib.parse.cdh_bin_parse import CDHSchedulerParser
from lib.parse.eps_bin_parse import EPSBinParser
from lib.output.mqtt_broker import MqttBroker

logging.basicConfig(level=logging.DEBUG, format="%(asctime)-15s %(levelname)s %(message)s")

input = SerialWrap("/dev/ttyUSB0", 9600, timeout=0.2)
cdhParserExtended = CDHBinParserExtended()
cdhSchedulerParser = CDHSchedulerParser()
epsParser = EPSBinParser()
mqtt = MqttBroker()
      
console = None      
def readBytes(n):
    bytes = bytearray()
    while len(bytes) < n:
        tmp = console.read(n - len(bytes))
        if len(tmp) == 0:
            return bytes
        bytes.extend(tmp)
    return bytes
        
def main():
    global console
    
    logging.info("ser2mqtt_cdh starting")
    
    console = serial.Serial(port="/dev/ttyUSB0", baudrate=9600, timeout=0.1)
    logging.info("Serial connected")
    
    while True:
        start = console.read(1)
        if len(start) == 0:
            continue
    
        start = start[0]
        bytes = bytearray()
        
        parser = None        
        if start == 0x80:
            bytes = readBytes(83 + 1)[:-1]
            if len(bytes) == 83:
                logging.info("Stats packet")
                parser = cdhParserExtended   
        elif start == 0x81:
            bytes = readBytes(27 + 1)[:-1]
            if len(bytes) == 27:
                logging.info("EPS packet")
                parser = epsParser
        elif start == 0x82:
            bytes = readBytes(2 + 1)[:-1]
            if len(bytes) == 2:
                logging.info("Scheduler packet")
                parser = cdhSchedulerParser
        else:
            bytes = console.readline()
        
        if parser != None:
            for (key, value) in parser.parse_packet(bytes):
                    mqtt.publish(key, value)
                    
        else:    
            bytes = bytearray(bytes)
            bytes.insert(0, start)
            
            try:
                line = bytes.decode().rstrip()
                logging.info("String: %s" % (line))
                mqtt.publish("CDH-raw", line)
            except:
                logging.info("Bytes: %s" % (bytes))
                mqtt.publish("CDH-raw", str(bytes))
        
    
    ################
    #for packet in input.get_packets(binary=True):        
    #    if len(packet) == 0:
    #        return
    #        
    #    parser = None
    #    if packet[0] == 0x80 and len(packet) == 85:
    #        logging.info("Stats packet %i B" % len(packet))
    #        parser = cdhParserExtended
    #        
    #    if packet[0] == 0x81 and len(packet) == 29:
    #        logging.info("Eps packet %i B" % len(packet))
    #        parser = epsParser
    #        
    #    if packet[0] == 0x82 and len(packet) == 4:
    #        logging.info("Scheduler packet %i B" % len(packet))
    #        parser = cdhSchedulerParser
    #        
    #    if parser != None:
    #        for (key, value) in parser.parse_packet(packet[1:-1]):
    #            mqtt.publish(key, value)
    #    
    #    else:
    #        if type(packet) is str:
    #            logging.info("String: %s" % (packet))
    #            mqtt.publish("CDH-raw", packet)
    #        else:
    #            packet = bytes(packet)
    #            logging.info("Bytes: %s" % (packet))
    #            mqtt.publish("CDH-raw", str(packet))
    
if __name__ == '__main__':
    main()