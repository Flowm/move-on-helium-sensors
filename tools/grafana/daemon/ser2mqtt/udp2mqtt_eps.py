#!/usr/bin/env python3

import logging
from lib.input.udp_server import UdpServer
from lib.parse.eps_bin_parse import EPSBinParser
from lib.output.mqtt_broker import MqttBroker

logging.basicConfig(level=logging.DEBUG, format="%(asctime)-15s %(levelname)s %(message)s")

input = UdpServer(23001)
parser = EPSBinParser()
output = MqttBroker()


def main():
    logging.info("udp2mqtt_eps starting")
    
    for packet in input.get_packets():
        for (key, value) in parser.parse_packet(packet[2:]):
            logging.info("Pub %s: %s" % (key, value))
            output.publish(key, value)


if __name__ == '__main__':
    main()
