#!/usr/bin/env python3

import logging
import sys
from lib.input.udp_server import UdpServer
from lib.parse.eps_bin_parse import EPSBinParser
from lib.output.mqtt_broker import MqttBroker

logging.basicConfig(level=logging.DEBUG, format="%(asctime)-15s %(levelname)s %(message)s")


def main():
    if len(sys.argv) < 2:
        logging.info("udp2mqtt_eps missing board argument")
        return        
        
    parser = EPSBinParser()
    output = MqttBroker()
        
    parser.board = int(sys.argv[1])
    if parser.board != 1 and parser.board != 2:
        logging.info("wrong board argument (not 1 or 2)")
        return
    
    logging.info("udp2mqtt_eps(%i) starting", parser.board)
    
    input = UdpServer(23001 if parser.board == 1 else 23002)
    
    for packet in input.get_packets():
        output.publish("CDH/EPS-COMPAKET", packet[-1])
        for (key, value) in parser.parse_packet(packet[2:2+26]):
            logging.info("Pub %s: %s" % (key, value))
            output.publish(key, value)


if __name__ == '__main__':
    main()
