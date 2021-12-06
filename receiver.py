import serial

class LiFiReceiver:
    """A Li-Fi receiver"""
    port = serial.Serial('/dev/ttyACM0')
    packet_capacity = 256
    bits_per_packet = 16
    period = 9
    high_threshold = 50
    low_threshold = 30
    start_pattern = '11000'
    end_pattern = '0110101101'
    halt_pattern = '0010000100'

    def listen_packet(self):
        packet = []
        if self.port.is_open:
            for _ in range(self.packet_capacity):
                signal = self.port.read()
                packet.append(int.from_bytes(signal, "little"))
        return packet

    def process_packet(self, packet):
        for i in range(len(packet)):
            if packet[i] < self.low_threshold:
                packet[i] = 0
            elif packet[i] > self.high_threshold:
                packet[i] = 1
            else:
                packet[i] = 0
        data = ''
        for i in range(int(self.period / 2), len(packet), self.period):
            data += str(packet[i])
        begin = data.find(self.start_pattern) + len(self.start_pattern)
        end = begin + self.bits_per_packet
        return data[begin:end]

    def listen(self):
        data = ''
        while True:
            bits = self.process_packet(self.listen_packet())
            print(bits)
            data += bits
            if bits.find(self.halt_pattern) != -1:
                break
        return data[:data.rfind(self.end_pattern)]
