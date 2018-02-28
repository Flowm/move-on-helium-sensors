#!/usr/bin/env python3
import time

import Adafruit_ADS1x15
from mqtt import MqttBroker

adc = Adafruit_ADS1x15.ADS1115()

mqtt = MqttBroker()

GAIN = 1

print('Reading ADS1x15 values, press Ctrl-C to quit...')
print('| {0:>6} | {1:>6} | {2:>6} | {3:>6} |'.format(*range(4)))
print('-' * 37)
while True:
    values = [0]*4
    for i in range(4):
        values[i] = adc.read_adc(i, gain=GAIN) / (2**15) * 4.096
        
    for i in range(4):
        mqtt.publish("power/voltage_%i" % i, str(values[i]))
    
    time.sleep(0.5)
