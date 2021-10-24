from gpiozero import MCP3008
from decode import decode
# from lifi import receive
from time import sleep

class LiFiReceiver:
    def __init__(self, adc):
        self._adc = adc
        self._low = 0
        self._high = 1
        self._med = (self._low + self._high) / 2

    def calibrate(self):
        signals = []
        for i in range(10000):
            signal = self._adc.value
            signals.append(signal)
        signals.sort()
        self._low = sum(signals[:1000]) / 1000
        self._high = sum(signals[-1000:]) / 1000
        self._med = (self._low + self._high) / 2

    def calibrate2(self):
        self._low = self._adc.value
        while True:
            current = self._adc.value
            if current >= 5 * self._low:
                self._high = current
                break
        self._med = (self._low + self._high) / 2
        print('done')
        

    def read(self):
        
        signal = self._adc.value
        if signal < self._med:
            return 0
        return 1


if __name__ == '__main__':
    adc = MCP3008(0)
    receiver = LiFiReceiver(adc)
    receiver.calibrate2()
    bits = []
    zero_count = 0
    while zero_count < 4:
        bit = receiver.read()
        if bit == 0:
            zero_count += 1
        else:
            zero_count = 0
        # print(receiver.read())
        bits.append(str(bit))
        sleep(0.2)
    print('============================================')
    print(bits)
    print('============================================')
    len_bits = bits[10:16]
    bit_string = ''
    for bit in len_bits:
        bit_string += bit
    print(bit_string)
    text_bits_num = int(bit_string, 2) * 16
    print(text_bits_num)
    text_bits = bits[16:(16 + text_bits_num)]
    decode(text_bits)

    
    