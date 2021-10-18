import sys
from send import send
from lifi import receive
from multiprocessing import Process

if __name__ == '__main__':
    # p1 = Process(target=receive(0.01))
    # p1.start()
    # sleep(0.1)
    # p2 = Process(target=send('test'))
    # p2.start()
    # p2.join()
    # p1.join()
    print(sys.argv[1])
    exit(0)