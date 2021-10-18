from time import sleep
from encode import encode
from transmit import addHeader, led_control
import sys

def send(text):
    bits, textLen = encode(text)
    bitsList = addHeader(1, int(textLen), bits)
    print(bitsList)
    print('bitlist len = ' + str(len(bitsList)))
    led_control(bitsList)


if __name__ == '__main__':
    if len(sys.argv) <= 1:
        sys.stderr.write('Should input at least one argument')
        sys.exit(1)
    send(sys.argv[1])