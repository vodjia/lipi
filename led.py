import RPi.GPIO as GPIO
import time
GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
GPIO.setup(18, GPIO.OUT)
GPIO.setup(15, GPIO.OUT)
t = 0.1
while True:
    GPIO.output(18, GPIO.HIGH)
    time.sleep(t)
    GPIO.output(18, GPIO.LOW)
    time.sleep(t)
