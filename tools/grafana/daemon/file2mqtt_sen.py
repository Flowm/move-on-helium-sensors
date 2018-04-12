#!/usr/bin/env python3

import argparse
import logging
from lib.input.file_wrap import FileWrap
from lib.parse.sen_bin_parse import SenBinParse
from lib.output.mqtt_broker import MqttBroker

logging.basicConfig(level=logging.DEBUG, format="%(asctime)-15s %(levelname)s %(message)s")

parser = SenBinParse()
output = MqttBroker()


def main():
    argparser = argparse.ArgumentParser(description="Parse com groundstation logs for sensor data")
    argparser.add_argument("filename")
    args = argparser.parse_args()

    logging.info("Reading %s" % args.filename)
    input = FileWrap(0, args.filename)

    for packet in input.get_packets():
        logging.debug("Packet: %s" % packet)

        for (key, value) in parser.parse_packet(packet):
            logging.info("Pub %s: %s" % (key, value))
            output.publish(key, value)


if __name__ == '__main__':
    main()
