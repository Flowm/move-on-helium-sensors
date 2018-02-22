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
    console = serial.Serial("/dev/ttyUSB0", 9600, timeout=1)
    logging.info("Serial connected")


subsystemNames = ["SENS", "EPS1", "EPS2", "CAM", "PAYL"]
subsystemResultNames = ["CF", "TR", "TH", "IS", "IN", "TD", "TF", "IE", "IC", "OK"]
subsystemResultOrder = [9, 8, 1, 2, 5, 6, 3, 4, 7, 0]
comResultNames = ["OK", "NR"] 

N = len(subsystemNames)
NR = len(subsystemResultNames)
NC = len(comResultNames)

structFormat = "".join([
            "L", # time
            "H", # cpu time
            "B", # sdUSed
            "B", # status
            "%iH" % N, # bytes
            "%iB" % (N * NR), # subsystem
            "%iB" % (N * NC) #com
            ])

class Stats:
    def __init__(self, bytes):
        fields = list(struct.unpack(structFormat, bytes))
        self.time = datetime.timedelta(milliseconds=fields.pop(0))
        self.cpuTime = fields.pop(0)
        self.sdUsed = fields.pop(0)
        self.status = fields.pop(0)
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
    result += "timestamp:    %s\n" % str(stats.time)
    result += "active time:  %i ms\n" % stats.cpuTime
    result += "used memory:  %02.2f %%\n" % (stats.sdUsed * 100)
    result += "status:       0x%.2X\n" % stats.status
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
    bytes = console.readline()
    
    try:
        if len(bytes) == 0:
            return
        bytes = bytes.strip()
        if len(bytes) == 0:
            client.publish("CDH", " ")
        
        if len(bytes) > 0 and bytes[0] == 0x04:
            bytes = bytes[1:]
            if len(bytes) == 78:
                stats = Stats(bytes)
                output = formatStats(stats)
                client.publish("CDH", output)
        else:
            line = bytes.decode()
            client.publish("CDH", line)
    except UnicodeDecodeError:
        logging.error(traceback.format_exc())
        logging.debug(bytes)

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
