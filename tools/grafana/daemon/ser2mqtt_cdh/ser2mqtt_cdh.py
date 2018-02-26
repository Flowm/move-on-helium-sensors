#!/usr/bin/env python3

import logging
import sys
import traceback
import struct
import datetime
import time
import paho.mqtt.client as mqtt
import serial

MQTT_IP = "localhost"
MQTT_PORT = 1883

logging.basicConfig(level=logging.DEBUG, format="(%(threadName)-10s) %(levelname)s %(message)s")
logging.info("ser2mqtt CDH starting")

def on_connect(client, userdata, flags, rc):
    logging.info("MQTT connected with result code " + str(rc))

client = None
def startMQTT():
    global client
    client = mqtt.Client(client_id="ser2mqttCDH")
    client.on_connect = on_connect
    client.connect(MQTT_IP, MQTT_PORT, 60)
    client.loop_start()
    logging.info("MQTT started")

console = None
def connectSerial():
    global console
    console = serial.Serial(port="/dev/ttyUSB0", baudrate=9600, timeout=10)
    logging.info("Serial connected")


subsystemNames = ["SENS", "EPS1", "EPS2", "CAM", "PAYL"]
subsystemResultNames = ["CF", "TR", "TH", "IS", "IN", "TD", "TF", "IE", "IC", "OK"]
subsystemResultOrder = [9, 8, 1, 2, 5, 6, 3, 4, 7, 0]
comResultNames = ["OK", "NR"] 

N = len(subsystemNames)
NR = len(subsystemResultNames)
NC = len(comResultNames)

structFormat = "".join([
            "L", # timestamp
            "H", # statsDelta
            "B", # activeTime
            "B", # usedMemory
            "%iH" % N, # bytes
            "%iB" % (N * NR), # subsystem
            "%iB" % (N * NC) #com
            ])

class Stats:
    def __init__(self, bytes):
        fields = list(struct.unpack(structFormat, bytes))
        self.timestamp= datetime.timedelta(milliseconds=fields.pop(0))
        self.statsDelta = fields.pop(0)
        self.activeTime = fields.pop(0) / 255.0
        self.usedMemory = fields.pop(0) / 255.0
        self.bytes = [0 for i in range(0, N)]
        self.subsystemResults = [[0 for j in range(0, NR)] for i in range(0, N)]
        self.comResults = [[0 for j in range(0, NC)] for i in range(0, N)]
        
        for i in range(0, N):
            self.bytes[i] = fields.pop(0)
        for i in range(0, N):
            for j in range(0, NR):
                self.subsystemResults[i][j] = fields.pop(0)
        for i in range(0, N):
            for j in range(0, NC):
                self.comResults[i][j] = fields.pop(0)           
                
def formatTable(data, rowNames, columnNames, title):
    n = len(data)
    m = len(data[0])
    
    result = ""
    result += (title + " ").ljust(8 + m * 4, "=") + "\n"
    result += " " * 8
    for j in range(0, m):
        result += "%-3s " % columnNames[j]
    result += "\n"
    for i in range(0, n):
        result += "%4s:   " % rowNames[i]
        for j in range(0, m):
            v = data[i][j]
            if v > 0:
                result += "%-3i " % v
            else:
                result += ".   "
        result += "\n"
    return result
                
def formatStats(stats):
    result = "\n"
    result += "STATS ".ljust(28, "=") + "\n"
    result += "timestamp:    %s\n" % str(stats.timestamp)
    result += "stats delta:  %i ms\n" % stats.statsDelta
    result += "active time:  %02.2f %%\n" % (stats.activeTime * 100)
    result += "used memory:  %02.2f %%\n" % (stats.usedMemory * 100)
    result += "\n"
    
    result += formatTable(stats.subsystemResults, subsystemNames, subsystemResultNames, "SUBSYSTEM RESULTS")
    result += "\n"
    
    result += "RECEIVED ".ljust(12, "=") + "\n"
    for i in range(0, N):
        result += "%4s:   " % subsystemNames[i]
        result += "%i" % stats.bytes[i]
        result += "\n"
    result += "\n"
    
    result += formatTable(stats.comResults, subsystemNames, comResultNames, "COM RESULTS")
    
    return result[:-1]
        
def handle_input():
    start = console.read(1)
    if len(start) == 0:
        return

    start = start[0]
    if  start == 0x04:
        bytes = console.read(78)
        stats = Stats(bytes)
        output = formatStats(stats)
        client.publish("CDH", output)
    else:
        bytes = bytearray(console.readline())
        bytes.insert(0, start)
        try:
            line = bytes.decode().rstrip()
            client.publish("CDH", line)
        except UnicodeDecodeError:
            client.publish("CDH", str(bytes)) 

while True:
    if client == None:
        startMQTT()
        
    if console == None:
        connectSerial()
        
    while True:
        try:
            handle_input()
        except serial.serialutil.SerialException:
            console = None
            logging.error(traceback.format_exc())
            break
        except (KeyboardInterrupt, SystemExit):
            raise
        except:
            console = None
            client = None
            logging.error(traceback.format_exc())
            break
