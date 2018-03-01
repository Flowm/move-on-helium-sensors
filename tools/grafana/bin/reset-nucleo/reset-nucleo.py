#!/usr/bin/env python3
# Reset nucleo by toggeling the reset line connected to BCM PIN 17

import RPi.GPIO as GPIO
import time

RST_PIN = 17

GPIO.setmode(GPIO.BCM)
GPIO.setup(RST_PIN, GPIO.OUT)

GPIO.output(RST_PIN, GPIO.LOW)
time.sleep(0.25)
GPIO.output(RST_PIN, GPIO.HIGH)

GPIO.setup(RST_PIN, GPIO.IN)
GPIO.cleanup()
