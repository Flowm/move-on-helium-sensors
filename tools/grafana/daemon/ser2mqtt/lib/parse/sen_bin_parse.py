import time
import logging
import json
from subprocess import Popen, PIPE, STDOUT
from lib.parse.sen_ascii_parse import SenAsciiParse


class SenBinParse:
    def __init__(self):
        self.ascii_parser = SenAsciiParse()

    def parse_packet(self, packet):
        logging.debug("BIN IN: %s" % packet)

        #TODO Call c parser here
        parser = Popen(['grep', '='], stdout=PIPE, stdin=PIPE, stderr=STDOUT)
        stdout = parser.communicate(input=packet)[0]

        logging.debug("BIN OUT: %s" % stdout.strip().decode())

        for line in stdout.decode().splitlines():
            for data in self.ascii_parser.parse_packet(line):
                yield data
