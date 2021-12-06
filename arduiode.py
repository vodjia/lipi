import serial
import matplotlib.pyplot as plt

packet_capacity = 512

photodiode = serial.Serial('/dev/ttyACM0')

def test(packet):
    for i in range(2, packet_capacity):
        # if abs(packet[i] - packet[i - 1]) < 20:
        #     return False
        if packet[i] < packet[i - 1] and packet[i - 1] < packet[i - 2]:
            return False
        if packet[i] < packet[i - 1] and packet[i - 1] < packet[i - 2]:
            return False
    return True

while True:
    packet = []
    if photodiode.is_open:
        for i in range(packet_capacity):
            reading = photodiode.read()
            packet.append(int.from_bytes(reading, "little"))
        print(packet)
        print(test(packet))
        # plt.figure()
        # plt.stem(packet)
        # plt.show()