#!/usr/bin/env python3

import logging
import sys
import paho.mqtt.client as mqtt
from serial_wrap import SerialWrap

MQTT_IP = "localhost"
MQTT_PORT = 1883

logging.basicConfig(level=logging.DEBUG, format="(%(threadName)-10s) %(levelname)s %(message)s")
logging.info("ser2mqtt starting")

def on_connect(client, userdata, flags, rc):
    logging.info("MQTT connected with result code " + str(rc))

client = mqtt.Client(client_id="ser2mqtt")
client.on_connect = on_connect
client.connect(MQTT_IP, MQTT_PORT, 60)
client.loop_start()

serial_console = SerialWrap("/dev/ttyACM0", 9600, timeout=0.2)

def handle_input():
    line = serial_console.try_readline_decode()
    if not line:
        return
    logging.debug("RECV: " + line)

    client.publish("sensors-raw", line)

    try:
        type, data = line.split(' ')
        for entry in data.split(','):
            try:
                name, value = entry.split('=')
            except ValueError:
                return
            else:
                client.publish("sensors/" + type + "/" + name, value)
                logging.info("Publish " + "sensors/" + type + "/" + name + " Value: " +  value)
    except ValueError:
        logging.info('Invalid data in line: ' + line)
        return


while True:
    handle_input()
