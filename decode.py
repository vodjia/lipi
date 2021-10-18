import sys


def decode(argv):
    manchester_bits = argv # .strip('][').split(', ')
    print('manchester bits len = ' + str(len(manchester_bits)) + '\n')
    print('manchester bits = ' + str(manchester_bits) + '\n')
    print('===============================================')
    bits = ''
    result = ''
    for i in range(0, len(manchester_bits), 2):
        if manchester_bits[i] == '1' and manchester_bits[i+1] == '0':
            bits += '1'
        elif manchester_bits[i] == '0' and manchester_bits[i+1] == '1':
            bits += '0'
        else:
            sys.stderr.write('bit error detected ')
            sys.stderr.write('i = ' + str(i))
            sys.exit(1)
    for i in range(0, len(bits), 8):
        result += chr(int(bits[i:i+8], 2))
    print(result)
    # sys.stdout.write()


if __name__ == '__main__':
    if len(sys.argv) <= 1:
        sys.stderr.write('Should input at least one argument')
        sys.exit(1)
    decode(sys.argv[1])
