import gpiozero
import sys
import RPi.GPIO as GPIO
import time


def addHeader(receiverId, textLength, encodedText):
    testBitsList = encodedText.strip('][').replace("'", "").split(', ')
    if int(receiverId) >= 4:
        print('Receiver id goes over the upperbond')
    elif int(textLength) >= 64:
        print('Text length goes over the upperbond')

    binaryId = bin(int(receiverId)).replace('0b','')
    while len(binaryId) < 2:
        binaryId = '0' + binaryId
    binaryLength = bin(int(textLength)).replace('0b','')
    while len(binaryLength) < 6:
        binaryLength = '0' + binaryLength
    header = binaryId + binaryLength
    print(header)
    manchester_header = ['1','0','1','0','1','0','1','0']
    for char in header:
        if char == '0':
            manchester_header.append('0')
        if char == '1':
            manchester_header.append('1')
    return manchester_header + testBitsList

def led_control(bitsList):
    GPIO.setmode(GPIO.BCM)
    GPIO.setwarnings(False)
    GPIO.setup(18, GPIO.OUT)
    GPIO.setup(15, GPIO.OUT)
    t = 0.2
    for bit in bitsList:
        if bit == '1':
            GPIO.output(18, GPIO.HIGH)
            time.sleep(t)
        else:
            GPIO.output(18, GPIO.LOW)
            time.sleep(t)
    GPIO.output(18, GPIO.LOW)
    

if __name__ == '__main__':
    if len(sys.argv) != 4:
        sys.stderr.write('Should input three parameters')
        sys.exit(1)
    bitsList = addHeader(sys.argv[1], sys.argv[2], sys.argv[3])
    led_control(bitsList)