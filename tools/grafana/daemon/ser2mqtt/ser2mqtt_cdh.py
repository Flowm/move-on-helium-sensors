#!/usr/bin/env python3

import logging
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
        
def main():
    logging.info("ser2mqtt_cdh starting")
    
    for packet in input.get_packets(binary=True):        
        if len(packet) == 0:
            return
            
        parser = None
        if packet[0] == 0x80 and len(packet) == 85:
            logging.info("Stats packet %i B" % len(packet))
            parser = cdhParserExtended
            
        if packet[0] == 0x81 and len(packet) == 29:
            logging.info("Eps packet %i B" % len(packet))
            parser = epsParser
            
        if packet[0] == 0x82 and len(packet) == 4:
            logging.info("Scheduler packet %i B" % len(packet))
            parser = cdhSchedulerParser
            
        if parser != None:
            for (key, value) in parser.parse_packet(packet[1:-1]):
                mqtt.publish(key, value)
        
        else:
            if type(packet) is str:
                logging.info("String: %s" % (packet))
                mqtt.publish("CDH-raw", packet)
            else:
                packet = bytes(packet)
                logging.info("Bytes: %s" % (packet))
                mqtt.publish("CDH-raw", str(packet))
    
if __name__ == '__main__':
    main()