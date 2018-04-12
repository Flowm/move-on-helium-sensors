import logging


class FileWrap():
    def __init__(self, channel, filename):
        self.channel = channel
        self.filename = filename

    def get_packets(self):
        with open(self.filename, "r") as file:
            for line in file:
                searchstr = "Sending to channel %d..." % self.channel
                if searchstr in line:
                    hex_str = line.split(searchstr)
                    if len(hex_str) == 2:
                        hex_arr = hex_str[1].split(", ")
                        byte_arr = []
                        for byte in hex_arr:
                            byte_arr += bytes.fromhex(byte[2:].strip())
                        yield bytes(byte_arr)
