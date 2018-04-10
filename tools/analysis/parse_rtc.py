#!/usr/bin/env python3

import argparse
import datetime
import logging
import re

logging.basicConfig(level=logging.DEBUG, format="%(asctime)-15s %(levelname)s %(message)s")


def main():
    argparser = argparse.ArgumentParser(description="Add timestamps to sensors log based on reference time")
    argparser.add_argument("filename")
    args = argparser.parse_args()

    logging.info("Reading %s" % args.filename)
    read_file(args.filename)


def read_file(filename):
    with open(filename, "r") as file:
        for line in file:
            line = line.strip()
            print(line)
            match = re.findall(r",RTC_S=(\d+),", line)
            if match and len(match) == 1:
                parse_time(match[0])


def parse_time(time_str):
    reference_rtc = 6572  # at 2018-04-04 14:00:18 CEST
    reference_date = datetime.datetime(2018, 4, 4, 12, 0, 18)  # UTC
    #print(reference_date.time())

    rtc = int(time_str)
    if rtc <= 0 or rtc > 20000:
        # Discard invalid rtc timestamps
        return
    delta_time = rtc - reference_rtc
    rtc_time = reference_date + datetime.timedelta(0, delta_time)
    rtc_time_cest = rtc_time + datetime.timedelta(0, 7200)
    rtc_time_str = rtc_time_cest.time()
    rtc_time_unix = int(rtc_time.strftime("%s"))
    print("TIME TIME=%s,UNIX=%d,RTC=%d" % (rtc_time_str, rtc_time_unix, rtc))


if __name__ == '__main__':
    main()
