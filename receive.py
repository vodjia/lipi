from decode import decode4b5b
from adc0820 import ADC0820
import time

period = 1000000
ns_in_s = 1000000000

upper = 120

receiver = ADC0820(
    db0=17, db1=27, db2=22, db3=23,
    db4=24, db5=25, db6=5, db7=6,
    rd_n=18, int_n=19, cs_n=20, ofl_n=21,
    wr_n=16, mode=26
)

listening = False
packet = ''

while True:
    time.sleep((period - time.time_ns() % period) / ns_in_s)
    raw = receiver.read()
    if raw >= upper:
        bit = '1'
    else:
        bit = '0'
    if bit == '1' and len(packet) == 0:
        listening = True
    if listening:
        packet += bit
    if len(packet) % 5 == 0 and packet[-5:-1] == '01101':
        listening = False
        print(packet)
        break
