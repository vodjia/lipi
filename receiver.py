class Receiver:
    def __init__(self, port, packet_capacity=256, bits_per_packet=16,
                 period=9, start_pattern='00110', end_pattern='01101',
                 halt_pattern='00100'):
        self.port = port
        self.packet_capacity = packet_capacity
        self.bits_per_packet = bits_per_packet
        self.period = period
        self.start_pattern = start_pattern
        self.end_pattern = end_pattern
        self.halt_pattern = halt_pattern

    def listen_raw_packet(self):
        packet = []
        if self.port.is_open:
            for _ in range(self.packet_capacity):
                signal = self.port.read()
                packet.append(int.from_bytes(signal, "little"))
        return packet

    def process_raw_packet(self, packet):
        data = ''
        start = int(0.75 * self.period)
        for i in range(start, len(packet), self.period):
            data += str(packet[i])
        begin = data.find(self.start_pattern) + len(self.start_pattern)
        end = begin + self.bits_per_packet
        return data[begin:end]

    def listen(self):
        data = ''
        while True:
            packet = self.process_raw_packet(self.listen_raw_packet())
            print(packet)
            data += packet
            if packet[: len(self.halt_pattern)] == self.halt_pattern:
                break
        return data[: data.rfind(self.end_pattern)]
