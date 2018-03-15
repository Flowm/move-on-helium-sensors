#!/usr/bin/env python3

import logging
import json
from lib.input.serial_wrap import SerialWrap
from lib.output.mqtt_broker import MqttBroker

logging.basicConfig(level=logging.DEBUG,
                    format="(%(threadName)-10s) %(levelname)s %(message)s")
logging.info("ser2mqtt starting")

mqtt = MqttBroker()
serial_console = SerialWrap("/dev/ttyACM0", 115200, timeout=0.2)


def str2digit(value):
    if value == "nan":
        return None
    try:
        return int(value)
    except ValueError:
        try:
            return float(value)
        except ValueError:
            return None


def handle_input():
    line = serial_console.try_readline_decode()
    if not line:
        return
    logging.debug("RECV: " + line)

    mqtt.publish("sensors-raw", line)

    try:
        data_dict = {}
        type, data = line.split(' ')
        for entry in data.split(','):

            try:
                name, value = entry.split('=')
            except ValueError:
                return
            else:
                data_dict[name] = str2digit(value)
                mqtt.publish("sensors/%s/%s" % (type, name), value)
                logging.info("Pub sensors/%s/%s: %s" % (type, name, value))
        if type == "GPS":
            mqtt.publish("sensors-processed/%s" % type, json.dumps(data_dict))
    except ValueError:
        logging.info('Invalid data in line: ' + line)
        return


def main():
    while True:
        handle_input()


if __name__ == '__main__':
    main()
