import logging
import socket
import sys


class UdpServer():
    def __init__(self, port=12000, host=''):
        self.host = host
        self.port = port

        # Open UDP Socket
        try:
            self.socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
            logging.info('Socket created')
        except socket.error:
            logging.info('Failed to create socket. Error Code : ' + str(msg[0]) + ' Message ' + msg[1])
            sys.exit()

        # Bind socket to local host and port
        try:
            self.socket.bind((self.host, self.port))
        except socket.error:
            logging.info('Bind failed. Error Code : ' + str(msg[0]) + ' Message ' + msg[1])
            sys.exit()

        logging.info('Socket bind complete')

    def __exit__(self, exc_type, exc_value, traceback):
        self.socket.close()

    def get_packets(self):
        while 1:
            (data, addr) = self.socket.recvfrom(256)
            if not data:
                continue
            length = len(data)
            logging.debug("RECV %3d: %s" % (length, data))

            if(data[length-1] != 0x00):
                logging.warning("At least one fragment is corrupt -> check the last byte: Each bit set to 1 tells which fragment is corrupt (LSB equals the first fragment, MSB the last fragment)")
                continue

            yield(data)
