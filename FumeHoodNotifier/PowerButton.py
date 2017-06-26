#!/usr/bin/env python

# Script to shudown RPi using a button
# Wire the button to pull GPIO 3 (Pin 5) to Ground to shutdown
# RPi has built-in function to wake RPi with the same button,
# so the button will both shutdown the RPi and wake it back up

import RPi.GPIO as GPIO
import subprocess

GPIO.setmode(GPIO.BCM)
GPIO.setup(3, GPIO.IN)
GPIO.wait_for_edge(3, GPIO.FALLING)

subprocess.call(['shutdown', '-h', 'now'], shell=False)
