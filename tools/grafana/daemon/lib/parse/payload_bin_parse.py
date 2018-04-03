import time
import logging
import struct
from subprocess import Popen, PIPE, STDOUT


class PayloadBinParser:
    def parse_packet(self, packet):
        timestamp = struct.unpack("<I", packet[0:4])[0]
        n = int(len(packet[4:]) / 2)
        for i in range(n):
            value = struct.unpack("<H", packet[4 + i * 2: 4 + (i+1) * 2])[0]
            #yield(str(timestamp + i * 1000), value)
            yield("payload/luminosity", value)
            break