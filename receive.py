import serial
from decode import decode4b5b
from receiver import Receiver

receiver = Receiver(serial.Serial('COM5'))
count = 0
num_errors = 0

while True:
    raw_data = receiver.listen()
    print(raw_data)
    data = decode4b5b(raw_data)
    print(data)
    if data != b'Hello World!':
        num_errors += 1
    count += 1
    print('count: ' + str(count))
    print('errors: ' + str(num_errors))
    print('error rate: ' + str(num_errors / count))
