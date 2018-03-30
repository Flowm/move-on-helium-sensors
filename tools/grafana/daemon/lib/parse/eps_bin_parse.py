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
    def __init__(self):
        self.board = 1

    def parse_packet(self, packet):
        fields = list(struct.unpack(epsStructFormat, packet))
        for i in range(len(fields)):
            value = fields[i]
            yield ("CDH/EPS%i/%s" %(self.board, epsStructNames[i]), value)