#!/usr/bin/env python3

import logging
import sys
from lib.input.udp_server import UdpServer
from lib.parse.payload_bin_parse import PayloadBinParser
from lib.output.mqtt_broker import MqttBroker

logging.basicConfig(level=logging.DEBUG, format="%(asctime)-15s %(levelname)s %(message)s")


def main():
    parser = PayloadBinParser()
    output = MqttBroker()
    
    logging.info("udp2mqtt_payload starting")
    
    input = UdpServer(23006)
    
    for packet in input.get_packets():
        output.publish("payload/COMPAKET", packet[-1])
        for (key, value) in parser.parse_packet(packet[2:-1]):
            logging.info("Pub %s: %s" % (key, value))
            output.publish(key, value)


if __name__ == '__main__':
    main()
