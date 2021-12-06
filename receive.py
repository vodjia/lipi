from decode import decode4b5b
from receiver import LiFiReceiver

receiver = LiFiReceiver()
data = receiver.listen()
print(data)
decode4b5b(data, './result.txt')
