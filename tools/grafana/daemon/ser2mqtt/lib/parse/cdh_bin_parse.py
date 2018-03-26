import time
import logging
import ctypes
import datetime
from subprocess import Popen, PIPE, STDOUT

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

    
class StatsExtended(ctypes.Structure):
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
        self.spinner = 0
        self.spinnerAnim = "|/-\\"
        
    def __str__(self):        
        result = "\n"
        result += "STATS ".ljust(28, "=")
        result += " "  + self.spinnerAnim[self.spinner] + "\n"
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
        
        self.spinner = (self.spinner + 1) % len(self.spinnerAnim)
        return result[:-1]

        
class StatsCompact(ctypes.Structure):
    _pack_ = 1
    _fields_ = [
        ("timestamp", ctypes.c_uint), 
        ("statsDeltaTime", ctypes.c_ushort),
        ("activeTime", ctypes.c_ubyte),
        ("usedMemory", ctypes.c_ubyte),
        ("requestResults", ctypes.c_ubyte * NumSubsystems),
        ("sendResults", ctypes.c_ubyte * NumSystems),
        ("loopCounter", ctypes.c_ubyte),
        ]
        
    def __init__(self, b):
        ctypes.memmove(ctypes.addressof(self), bytes(b), ctypes.sizeof(self))
        self.spinner = 0
        self.spinnerAnim = "|/-\\"
        
    def __str__(self):
        result = "\n"
        result += "STATS ".ljust(28, "=")
        result += " "  + self.spinnerAnim[self.spinner] + "\n"
        time = datetime.timedelta(milliseconds=self.timestamp)
        result += "timestamp:    %s\n" % str(time)
        result += "stats delta:  %i ms\n" % self.statsDeltaTime
        result += "active time:  %02.2f %%\n" % (self.activeTime / 255.0 * 100)
        result += "used memory:  %02.2f %%\n" % (self.usedMemory / 255.0 * 100)
        result += "loop count:  %i\n" % self.loopCounter
        result += "\n"
        
        result += "REQUEST RESULTS".ljust(12, "=") + "\n"
        for i in range(0, NumSubsystems):
            result += "%4s:   " % subsystemNames[i]
            result += "%i" % self.requestResults[i]
            result += "\n"
        result += "\n"
        
        result += "SEND RESULTS".ljust(12, "=") + "\n"
        for i in range(0, NumSystems):
            result += "%4s:   " % systemNames[i]
            result += "%i" % self.sendResults[i]
            result += "\n"
        result += "\n"
        
        self.spinner = (self.spinner + 1) % len(self.spinnerAnim)
        return result[:-1]

class CDHBinParserExtended:
    def parse_packet(self, packet):
        stats = StatsExtended(packet)
        output = str(stats)
        yield ("CDH-stats", output)
        yield ("CDH-raw", output)

class CDHBinParserCompact:
    def parse_packet(self, packet):
        stats = StatsCompact(packet)
        output = str(stats)
        yield ("CDH-stats", output)
        yield ("CDH-raw", output)
        
class CDHSchedulerParser:
    def parse_packet(self, packet):
        time = datetime.datetime.now()
        event = ''
        event += str(time) + ", "
        event += str(packet[0]) + ", "
        event += str(packet[1]) + "\n"
        yield ("CDH-scheduler", event)
        yield ("CDH-raw", event)
