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
    console = serial.Serial(port="/dev/ttyUSB0", baudrate=9600, timeout=0.1)
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

structFormatEPS = ">H" + ("Hhh" * 3) + "HHH"

structNamesEPS = [
           "Counter",
           "Solar_Cell_Voltage",
           "Solar_Cell_Current",
           "Solar_Cell_Power",
           "Battery_Voltage",
           "Battery_Current",
           "Battery_Power",
           "Bus_Voltage",
           "Bus_Current",
           "Bus_Power",
           "Chrg_PWN",
           "Exzess_PWM",
           "Temperature"
           ]
           


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
                

spinner = 0
spinnerAnim = "|/-\\"

def formatStats(stats):
    global spinner

    result = "\n"
    result += "STATS ".ljust(28, "=")
    result += " "  + spinnerAnim[spinner] + "\n"
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
    result += "\n"

    spinner = (spinner + 1) % len(spinnerAnim)
    return result[:-1]

def readBytes(n):
    bytes = bytearray()
    while len(bytes) < n:
        tmp = console.read(n - len(bytes))
        if len(tmp) == 0:
            return bytes
        bytes.extend(tmp)
    return bytes

def handle_input():
    start = console.read(1)
    if len(start) == 0:
        return

    start = start[0]
    bytes = bytearray()
    if  start == 0x80:
        bytes = readBytes(78)

        if len(bytes) == 78:
            stats = Stats(bytes)
            output = formatStats(stats)
            client.publish("CDH-raw", output)

            client.publish("CDH/active",stats.activeTime)
            client.publish("CDH/memory", stats.usedMemory)
            for i in range(N):
                ok = sum(stats.subsystemResults[i][8:10])
                n =  sum(stats.subsystemResults[i]) * 1.0
                avg = ok / n if n > 0 else 0
                client.publish("CDH/%s" % subsystemNames[i], str(avg))
            return
    elif start == 0x81:
        bytes = readBytes(27)
        if len(bytes) == 27:
            channel = bytes[0]

            fields = list(struct.unpack(structFormatEPS, bytes[1:]))
            for i in range(len(fields)):
                value = fields[i]
                client.publish("CDH/EPS%i/%s" %(channel, structNamesEPS[i]), value)
            return
    else:
        bytes = console.readline()
    
    bytes = bytearray(bytes)
    bytes.insert(0, start)
    try:
        line = bytes.decode().rstrip()
        client.publish("CDH-raw", line)

    except UnicodeDecodeError:
        client.publish("CDH-raw", str(bytes))

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
