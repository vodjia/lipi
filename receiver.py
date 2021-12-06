import serial
import numpy as np

class LiFiReceiver:
    """A Li-Fi receiver"""
    port = serial.Serial('/dev/ttyACM0')
    packet_capacity = 512
    period = 9

    def listen_packet(self):
        packet = []
        if self.port.is_open:
            for _ in range(self.packet_capacity):
                signal = self.port.read()
                packet.append(int.from_bytes(signal, "little"))
        return packet

    def process_packet(self, packet):
        packet_upper = np.percentile(packet, 90)
        packet_lower = np.percentile(packet, 10)
        packet_range = packet_upper - packet_lower
        packet_middle = packet_range / 2
        high_threshold = packet_middle + 0.4 * packet_range
        low_threshold = packet_middle - 0.4 * packet_range
        for i in range(len(packet)):
            if packet[i] < low_threshold:
                packet[i] = 0
            elif packet[i] > high_threshold:
                packet[i] = 1
            else:
                packet[i] = 0

    def listen_raw(self):
        raw_data = []
        while True:
            packet = self.listen_packet()
            self.process_packet(packet)
            raw_data.append(packet)

            if packet.count(0) == len(packet):
                break
        return raw_data

    def process_raw(self, raw_data):
        raw_data = ''.join(str(i) for i in raw_data)
        start = raw_data.find('111111111111111111000000000000000000000000000')
        data = ''
        for i in range(start + int(self.period / 2), len(raw_data), self.period):
            data += raw_data[i]
        return data[5:data.find('00000')]

    def listen(self):
        return self.process_raw(self.listen_raw())
