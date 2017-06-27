#!/usr/bin/env python

# Script to shudown RPi using a button
# Wire the button to pull GPIO 3 (Pin 5) to Ground to shutdown
# RPi has built-in function to wake RPi with the same button,
# so the button will both shutdown the RPi and wake it back up

import RPi.GPIO as GPIO
import subprocess
import time

powerPin = 3

GPIO.setmode(GPIO.BCM)
GPIO.setup(powerPin, GPIO.IN)

while True:
    GPIO.wait_for_edge(powerPin, GPIO.FALLING)
    buttonPressTime = time.time()
    while not(GPIO.input(powerPin)):
        currentTime = time.time()
        if currentTime - buttonPressTime >= 2:
            subprocess.call(['shutdown', '-h', 'now'], shell=False)
