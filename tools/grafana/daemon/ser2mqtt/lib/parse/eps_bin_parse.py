import time
import logging
import struct
from subprocess import Popen, PIPE, STDOUT

epsStructFormat = ">H" + ("Hhh" * 3) + "HHH"
epsStructNames = ["Counter",
           "Solar_Cell_Voltage", "Solar_Cell_Current", "Solar_Cell_Power",
           "Battery_Voltage", "Battery_Current", "Battery_Power", 
           "Bus_Voltage", "Bus_Current", "Bus_Power",
           "Chrg_PWN", "Exzess_PWM", "Temperature"]

class EPSBinParser:
    def parse_packet(self, packet):
        channel = packet[0]
        fields = list(struct.unpack(epsStructFormat, packet[1:]))
        for i in range(len(fields)):
            value = fields[i]
            yield ("CDH/EPS%i/%s" %(channel, epsStructNames[i]), value)