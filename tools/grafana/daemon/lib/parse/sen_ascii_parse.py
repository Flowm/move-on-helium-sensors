import logging
import json


def str2digit(value):
    if value == "nan":
        return None
    try:
        return int(value)
    except ValueError:
        try:
            return float(value)
        except ValueError:
            return None


class SenAsciiParse:
    def parse_packet(self, packet, topic="raw"):
        yield ("sensors-%s" % topic, packet)

        try:
            data_dict = {}
            type, data = packet.split(' ')
            for entry in data.split(','):

                try:
                    name, value = entry.split('=')
                except ValueError:
                    return
                else:
                    data_dict[name] = str2digit(value)
                    yield ("sensors/%s/%s" % (type, name), value)
            if type == "GPS":
                yield ("sensors-processed/%s" % type, json.dumps(data_dict))
        except ValueError:
            logging.info('Invalid data in packet: ' + packet)
            return
