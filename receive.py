from decode import decode4b5b
from receiver import LiFiReceiver

receiver = LiFiReceiver()
decode4b5b(''.join(receiver.listen()), './result.txt')
