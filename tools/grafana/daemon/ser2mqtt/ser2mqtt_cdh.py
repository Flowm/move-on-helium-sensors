#!/usr/bin/env python3

import logging
import sys
import os
import traceback
import struct
import ctypes
import datetime
import time
import paho.mqtt.client as mqtt
import serial
import threading

MQTT_IP = "localhost"
MQTT_PORT = 1883

logging.basicConfig(level=logging.DEBUG, format="%(asctime)-15s %(levelname)s %(message)s")
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
systemNames = subsystemNames + ["GPS", "CDH"]
requestResultNames = ["CF", "TR", "TH", "IS", "IN", "TD", "TF", "IE", "IC", "OK"]
sendResultNames = ["NR", "OK"] 

NumSubsystems = len(subsystemNames)
NumSystems = len(systemNames)
NumRequestResults = len(requestResultNames)
NumSendResults = len(sendResultNames)

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
    
class Stats(ctypes.Structure):
    _pack_ = 1
    _fields_ = [
        ("timestamp", ctypes.c_uint), 
        ("statsDeltaTime", ctypes.c_ushort),
        ("activeTime", ctypes.c_ubyte),
        ("usedMemory", ctypes.c_ubyte),
        ("bytes", ctypes.c_short * NumSubsystems),
        ("requestResults", ctypes.c_ubyte * NumRequestResults * NumSubsystems),
        ("sendResults", ctypes.c_ubyte * NumSendResults * NumSystems),
        ("loopCounter", ctypes.c_ubyte),
        ]
        
    def __init__(self, b):
        ctypes.memmove(ctypes.addressof(self), bytes(b), ctypes.sizeof(self))
        
    def __str__(self):
        global spinner
        
        result = "\n"
        result += "STATS ".ljust(28, "=")
        result += " "  + spinnerAnim[spinner] + "\n"
        time = datetime.timedelta(milliseconds=self.timestamp)
        result += "timestamp:    %s\n" % str(time)
        result += "stats delta:  %i ms\n" % self.statsDeltaTime
        result += "active time:  %02.2f %%\n" % (self.activeTime / 255.0 * 100)
        result += "used memory:  %02.2f %%\n" % (self.usedMemory / 255.0 * 100)
        result += "loop count:  %i\n" % self.loopCounter
        result += "\n"
        
        result += formatTable(self.requestResults, subsystemNames, requestResultNames, "REQUEST RESULTS")
        result += "\n"
        
        result += "RECEIVED ".ljust(12, "=") + "\n"
        for i in range(0, NumSubsystems):
            result += "%4s:   " % subsystemNames[i]
            result += "%i" % self.bytes[i]
            result += "\n"
        result += "\n"
        
        result += formatTable(self.sendResults, systemNames, sendResultNames, "SEND RESULTS")
        result += "\n"
        
        spinner = (spinner + 1) % len(spinnerAnim)
        return result[:-1]

def handle_cdh(bytes):
    stats = Stats(bytes)
    output = str(stats)
    client.publish("CDH-raw", output)

    client.publish("CDH/active", stats.activeTime / 255.0)
    client.publish("CDH/memory", stats.usedMemory / 255.0)
    for i in range(NumSubsystems):
        ok = stats.requestResults[i][-1]
        n =  sum(stats.requestResults[i]) * 1.0
        avg = ok / n if n > 0 else 0
        client.publish("CDH/%s" % subsystemNames[i], str(avg))

        
schedulerLogFile = None
schedulerLogFilename = "~/logs/cdh_scheduler.log"
schedulerLogFileMutex = threading.Lock()

def findSchedulerFilename():
    filename = os.path.expanduser(schedulerLogFilename)
    dir, tmp = os.path.split(filename)
    name, ext = os.path.splitext(tmp)
    suffix = 0
    if not os.path.isdir(dir):
        os.makedirs(dir)
    
    for f in os.listdir(dir):
        d, t = os.path.split(f)
        n, e = os.path.splitext(t)
        if n.startswith(name):
            try:
                i = int(n[len(name):])
                suffix = max(suffix, i + 1)
            except ValueError:
                pass
    filename = dir + "/" + name + str(suffix) + ext
    return filename
    
    
def openSchdulerLogFile():
    global schedulerLogFile 
    while True:
        filename = findSchedulerFilename()
        
        schedulerLogFileMutex.acquire()
        schedulerLogFile = open(filename, 'w')
        schedulerLogFileMutex.release()
        
        time.sleep(3600)
        
        schedulerLogFileMutex.acquire()
        schedulerLogFile.close()
        schedulerLogFileMutex.release()
    
def handle_cdh_scheduler(bytes):    
    time = datetime.datetime.now()
    event = ''
    event += str(time) + ", "
    event += str(bytes[0]) + ", "
    event += str(bytes[1]) + "\n"
    
    if schedulerLogFile != None:
        schedulerLogFileMutex.acquire()
        schedulerLogFile.write(event)
        schedulerLogFileMutex.release()
        
    client.publish("CDH-scheduler", event)
    
epsStructFormat = ">H" + ("Hhh" * 3) + "HHH"
epsStructNames = ["Counter",
           "Solar_Cell_Voltage", "Solar_Cell_Current", "Solar_Cell_Power",
           "Battery_Voltage", "Battery_Current", "Battery_Power", 
           "Bus_Voltage", "Bus_Current", "Bus_Power",
           "Chrg_PWN", "Exzess_PWM", "Temperature"]
    
def handle_eps(bytes):
    channel = bytes[0]
    fields = list(struct.unpack(epsStructFormat, bytes[1:]))
    for i in range(len(fields)):
        value = fields[i]
        client.publish("CDH/EPS%i/%s" %(channel, epsStructNames[i]), value)
    
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
        bytes = readBytes(83)
        if len(bytes) == 83:
            handle_cdh(bytes)
            return
    elif start == 0x82:
        bytes = readBytes(2)
        if len(bytes) == 2:
            handle_cdh_scheduler(bytes)
            return
    elif start == 0x81:
        bytes = readBytes(27)
        if len(bytes) == 27:
            handle_eps(bytes)
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
        
    
t = threading.Thread(target=openSchdulerLogFile)
t.start()
    
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
