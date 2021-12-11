import time

class Receiver:
    def __init__(self, device, period, threshold,
                 start_pattern='11000', end_pattern='01101'):
        self.device = device
        self.period = period
        self.threshold = threshold
        self.start_pattern = start_pattern
        self.end_pattern = end_pattern

    def receive_bit(self):
        ns_in_s = 1000000000
        delta_ns = 1.05 * self.period - time.time_ns() % self.period
        time.sleep(delta_ns / ns_in_s)
        if self.device.read() >= self.threshold:
            return '1'
        return '0'

    def receive_bits(self, count):
        bits = ''
        for i in range(count):
            bits += self.receive_bit()
        return bits

    def receive(self):
        bits = ''
        while True:
            pattern = self.receive_bits(len(self.start_pattern))
            if pattern == self.end_pattern:
                return bits
            bits += pattern
