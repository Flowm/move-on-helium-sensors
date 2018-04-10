#!/usr/bin/env python3

import argparse
import datetime
import logging
import re

logging.basicConfig(level=logging.DEBUG, format="%(asctime)-15s %(levelname)s %(message)s")


def main():
    argparser = argparse.ArgumentParser(description="Format sensors log for import into influxdb")
    argparser.add_argument("filename")
    args = argparser.parse_args()

    logging.info("Reading %s" % args.filename)

    print("# DDL")
    print("CREATE DATABASE telegraf")

    print("# DML")
    print("# CONTEXT-DATABASE:telegraf")
    read_file(args.filename)


def read_file(filename):
    with open(filename, "r") as file:
        last_line = {}
        last_time = None
        for line in file:
            line = line.strip()
            type = re.findall(r"^(\w+) ", line)
            if type:
                if type[0] == "DBG":
                    if last_time and len(last_line) > 1:
                        print_lines(last_line, last_time)
                    last_line = {}
                    last_time = None
                    continue

                last_line[type[0]] = line
                match = re.findall(r",UNIX=(\d+),", line)
                if match and len(match) == 1:
                    last_time = int(match[0])


def print_lines(lines, timestamp):
    for type in sorted(lines):
        if type != "TIME":
            print("%s %d" % (lines[type], timestamp))


if __name__ == '__main__':
    main()
