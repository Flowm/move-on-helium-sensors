#!/usr/bin/env python3

import logging
from mqtt import MqttBroker
from serial_wrap import SerialWrap

logging.basicConfig(level=logging.DEBUG,
                    format="(%(threadName)-10s) %(levelname)s %(message)s")
logging.info("ser2mqtt starting")

mqtt = MqttBroker()
serial_console = SerialWrap("/dev/ttyACM0", 115200, timeout=0.2)


def handle_input():
    line = serial_console.try_readline_decode()
    if not line:
        return
    logging.debug("RECV: " + line)

    mqtt.publish("sensors-raw", line)

    try:
        type, data = line.split(' ')
        for entry in data.split(','):
            try:
                name, value = entry.split('=')
            except ValueError:
                return
            else:
                mqtt.publish("sensors/%s/%s" % (type, name), value)
                logging.info("Pub sensors/%s/%s: %s" % (type, name, value))
    except ValueError:
        logging.info('Invalid data in line: ' + line)
        return


def main():
    while True:
        handle_input()


if __name__ == '__main__':
    main()
