#!/usr/bin/env python3

import logging
from lib.input.udp_server import UdpServer
from lib.parse.sen_ascii_parse import SENParse
from lib.output.mqtt_broker import MqttBroker

logging.basicConfig(level=logging.DEBUG,
                    format="(%(threadName)-10s) %(levelname)s %(message)s")
mqtt = MqttBroker()
udp = UdpServer(23000)
parser = SENParse()


def main():
    logging.info("ser2mqtt_sen_udp starting")

    while True:
        for packet in udp.try_readline_decode():
            if not packet:
                continue
            logging.debug("RECV: " + str(packet))

            #for (key, value) in parser.parse_packet(packet):
            #        logging.info("Pub %s: %s" % (key, value))
            #        mqtt.publish(key, value)


if __name__ == '__main__':
    main()
