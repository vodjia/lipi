import sys

def encode(argv):
    text = argv
    if len(text) >= 64 :
        print('input length goes over the higher bond')
        sys.exit(0)        
    text_bytes = bytearray(text, 'utf-8')
    text_bits = []
    manchester_bits = []
    for byte in text_bytes:
        bits = bin(byte).replace('0b','')
        while len(bits) < 8:
            bits = '0' + bits
        text_bits += list(bits)
        for bit in list(bits.replace('0b', '')):
            if bit == '1':
                manchester_bits.append('1')
                manchester_bits.append('0')
            elif bit == '0':
                manchester_bits.append('0')
                manchester_bits.append('1')
    result = str(manchester_bits)
    return result, len(text)


if __name__ == '__main__':
    if len(sys.argv) <= 1:
        sys.stderr.write('Should input at least one argument')
        sys.exit(1)
    encode(sys.argv[1])
