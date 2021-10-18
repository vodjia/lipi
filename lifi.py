from time import sleep
from gpiozero import MCP3008

def receive(interval):
    pd = MCP3008(0, max_voltage=3.3)
    while True:
        print(pd.voltage)
        sleep(interval)


if __name__ == '__main__':
    receive(0.2)


