import logging
import socket
import sys

# Initialize logging structure
logger = logging.getLogger('moveOn')
hdlr = logging.FileHandler('moveOn.log')
formatter = logging.Formatter('%(asctime)s %(levelname)s %(message)s')
hdlr.setFormatter(formatter)
logger.addHandler(hdlr)
logger.setLevel(logging.DEBUG)


class UdpServer():
    def __init__(self, port=8888, host=''):
        self.host = host
        self.port = port

        # Open UDP Socket
        try:
            self.socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
            print('Socket created')
        except socket.error:
            print('Failed to create socket. Error Code : ' + str(msg[0]) + ' Message ' + msg[1])
            sys.exit()

        # Bind socket to local host and port
        try:
            self.socket.bind((self.host, self.port))
        except socket.error:
            print('Bind failed. Error Code : ' + str(msg[0]) + ' Message ' + msg[1])
            sys.exit()

        print('Socket bind complete')

    def __exit__(self, exc_type, exc_value, traceback):
        self.socket.close()

    def try_readline_decode(self):
        # TODO: does not yield data yet
        while 1:
            (data, addr) = self.socket.recvfrom(256)
            data = d[0]
            addr = d[1]

            if not data:
                break

            print(data)
            counter = 0
            for i in data:
                counter = counter + 1
            #print(counter)
            if(data[counter-1] != 0x00):
                logger.warning("At least one fragment is corrupt -> check the last byte: Each bit set to 1 tells which fragment is corrupt (LSB equals the first fragment, MSB the last fragment)")
            logger.debug(data)
            yield(data)
