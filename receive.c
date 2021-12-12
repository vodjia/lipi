#include <stdio.h>
#include <stdlib.h>
#include "adc0820.h"
#include "receiver.h"
#include "decode.h"

#define BUFFER_CAPACITY 8192

int main(void)
{
	const char chipname[] = "gpiochip0";
	struct gpiod_chip *chip = gpiod_chip_open_by_name(chipname);
	struct adc0820_pinout pinout = {
		.db0 = 17, .db1 = 27, .db2 = 22, .db3 = 23,
		.wr_n = 16, .mode = 26, .rd_n = 18, .int_n = 19, .cs_n = 20,
		.db4 = 24, .db5 = 25, .db6 = 5, .db7 = 6, .ofl_n = 21
	};
	struct adc0820_device *device = adc0820_new(chip, &pinout);
	const unsigned long period = 1000;
	const unsigned long threshold = 120;
	const size_t code_length = 5;
	const char start_code = 0x18;
	const char end_code = 0x0D;
	struct lifi_receiver *receiver = lifi_receiver_new(device,
							   adc0820_read,
							   period,
							   threshold,
							   code_length,
							   start_code,
							   end_code);
	for (;;) {
		char code = lifi_receive_bit(receiver);
		if (code == 1) {
			code <<= code_length - 1;
			code |= lifi_receive_bits(receiver, code_length - 1);
			if (code == receiver->start_code) {
				char raw[BUFFER_CAPACITY] = { 0 };
				size_t length = 
					lifi_receive(receiver,
						     raw,
						     BUFFER_CAPACITY);
				printf("Raw: [");
				for (size_t i = 0; i < length; ++i)
					printf("%x, ", raw[i]);
				printf("]\n");
				char data[BUFFER_CAPACITY] = { 0 };
				int ret = decode4b5b(data, raw);
				if (ret < 0)
					printf("Error at index: %d\n", -ret);
				else
					printf("Data: %s\n", data);
			}
		}
	}
	lifi_receiver_delete(receiver);
	adc0820_delete(device);
	gpiod_chip_close(chip);
	return 0;
}
