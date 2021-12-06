import binascii
import sys

def decode4b5b(bits, save_path):
    file_data = bits[bits.find("11000")+5:-1]
    decode_dict = {
        "11110":"0",
        "01001":"1",
        "10100":"2",
        "10101":"3",
        "01010":"4",
        "01011":"5",
        "01110":"6",
        "01111":"7",
        "10010":"8",
        "10011":"9",
        "10110":"a",
        "10111":"b",
        "11010":"c",
        "11011":"d",
        "11100":"e",
        "11101":"f"
    }
    n = 0
    hex_str = ""
    while n < len(file_data) - 4:
        hex_str += decode_dict[file_data[n:n+5]]
    data = binascii.unhexlify(hex_str.endcode("ASCII"))
    with open(save_path, 'wb') as file:
        file.write(data)

if __name__ == '__main__':
    if len(sys.argv) <= 1:
        sys.stderr.write('Should input at least one argument')
        sys.exit(1)
    decode(sys.argv[1])
