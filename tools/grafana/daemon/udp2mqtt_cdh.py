#!/usr/bin/env python3

import logging
from lib.input.udp_server import UdpServer
from lib.parse.cdh_bin_parse import CDHBinParserCompact
from lib.output.mqtt_broker import MqttBroker

logging.basicConfig(level=logging.DEBUG, format="%(asctime)-15s %(levelname)s %(message)s")

def main():
    logging.info("udp2mqtt_cdh starting")
    
    input = UdpServer(23000)
    parser = CDHBinParserCompact()
    output = MqttBroker()
    
    for packet in input.get_packets():
        output.publish("CDH/CDH-COMPAKET", packet[-1])
        for (key, value) in parser.parse_packet(packet[2:2+21]):
            logging.info("Pub %s" % (key))
            output.publish(key, value)


if __name__ == '__main__':
    main()
