import logging
from subprocess import Popen, PIPE, STDOUT
from lib.parse.sen_ascii_parse import SenAsciiParse


class SenBinParse:
    def __init__(self):
        self.ascii_parser = SenAsciiParse()

    def parse_packet(self, packet, with_header=True):
        if len(packet) < 10:
            return

        length = len(packet)
        status = 0
        data = packet
        invalid_chunks = 0

        if with_header:
            length, status = packet[:2]
            data = packet[2:-1]
            invalid_chunks = packet[-1]
            logging.debug("BIN IN: CHK=%d" % invalid_chunks)

        #if invalid_chunks != 0:
        #    return

        parser = Popen(['moveon-sen-parser', str(invalid_chunks)], stdout=PIPE, stdin=PIPE, stderr=STDOUT)
        stdout = parser.communicate(input=data)[0]

        logging.debug("BIN OUT: %s" % stdout.strip().decode())

        for line in stdout.decode().splitlines():
            for data in self.ascii_parser.parse_packet(line, "com"):
                yield data
