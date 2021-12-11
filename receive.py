from adc0820 import ADC0820
from decode import decode4b5b
from receiver import Receiver

period = 10000000
start_pattern = '11000'

total_count = 0
error_count = 0

device = ADC0820(
    db0=17, db1=27, db2=22, db3=23,
    db4=24, db5=25, db6=5, db7=6,
    rd_n=18, int_n=19, cs_n=20, ofl_n=21,
    wr_n=16, mode=26
)

receiver = Receiver(device, period, 130)

while True:
    if receiver.receive_bit() == start_pattern[0]:
        if receiver.receive_bits(len(start_pattern) - 1) == start_pattern[1:]:
            total_count += 1
            raw = receiver.receive()
            data = decode4b5b(raw)
            if data != b'Hello World!\n':
                error_count += 1
                print(raw)
                print(data)
            print(raw)
            print('Total count: ' + str(total_count))
            print('Error count: ' + str(error_count))
            print('Error rate: ' + str(error_count / total_count))
