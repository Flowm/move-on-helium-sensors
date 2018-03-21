#!/usr/bin/env python3

import logging
from lib.input.serial_wrap import SerialWrap
from lib.parse.sen_ascii_parse import SENParse
from lib.output.mqtt_broker import MqttBroker

logging.basicConfig(level=logging.DEBUG, format="%(asctime)-15s %(levelname)s %(message)s")

input = SerialWrap("/dev/ttyACM0", 115200, timeout=0.2)
parser = SENParse()
mqtt = MqttBroker()


def main():
    logging.info("ser2mqtt_sen starting")

    for packet in input.get_packets():
        logging.debug("Packet: %s" % packet)

        for (key, value) in parser.parse_packet(packet):
                logging.info("Pub %s: %s" % (key, value))
                mqtt.publish(key, value)


if __name__ == '__main__':
    main()
