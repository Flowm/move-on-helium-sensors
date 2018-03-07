#!/usr/bin/env python3
import time

import Adafruit_ADS1x15
from mqtt import MqttBroker

adc = Adafruit_ADS1x15.ADS1115()

mqtt = MqttBroker()

GAIN = 1
diffs = [0, 3]

while True:
    values = [0] * 2

    for i in range(2):
        values[i] = adc.read_adc_difference(diffs[i], gain=GAIN)
        values[i] = values[i] * 4.096 / (2**15)
    
    for i in range(2):
        mqtt.publish("power/voltage_%i" % i, str(values[i]))
    
    time.sleep(0.5)
