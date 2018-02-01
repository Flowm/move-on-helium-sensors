#!/usr/bin/env python

import logging
import sys
import paho.mqtt.client as mqtt

MQTT_IP = "localhost"
MQTT_PORT = 1883

logging.basicConfig(level=logging.DEBUG, format="(%(threadName)-10s) %(levelname)s %(message)s")
logging.info("ser2mqtt starting")

def on_connect(client, userdata, flags, rc):
    print("Connected with result code " + str(rc))

client = mqtt.Client(client_id="ser2mqtt")
client.on_connect = on_connect
client.connect(MQTT_IP, MQTT_PORT, 60)
client.loop_start()

while True:
    line = sys.stdin.readline().strip()
    client.publish("sensors-raw", line)

    try:
        type, data = line.split(' ')
        for entry in data.split(','):
            try:
                name, value = entry.split('=')
            except ValueError:
                continue
            else:
                client.publish("sensors/" + type + "/" + name, value)
                print("Publish " + "sensors/" + type + "/" + name + " Value: " +  value)
    except ValueError:
        print('Invalid data in line: ' + line)
        continue
