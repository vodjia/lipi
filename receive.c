#include "adc0820.h"
#include "decode.h"
#include "receiver.h"
#include <stdio.h>
#include <stdlib.h>

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
	const char start_code = 0x06;
	const char end_code = 0x0D;
	struct lipi_config config = {
		.period = period,
		.threshold = threshold,
		.code_length = code_length,
		.start_code = start_code,
		.end_code = end_code
	};
	struct lipi_receiver *receiver =
		lipi_receiver_new(&config, device, adc0820_read);
	for (;;) {
		char code = lipi_receive_bit(receiver);
		if (code == start_code >> (code_length - 1)) {
			code <<= code_length - 1;
			code |= lipi_receive_bits(receiver, code_length - 1);
			if (code == start_code) {
				char raw[BUFFER_CAPACITY] = { 0 };
				size_t length = 
					lipi_receive(receiver,
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
				FILE *file = fopen("received.txt", "w");
				if (fputs(data, file) == EOF) {
					perror("Failed to write.");
					return -1;
				}
				fclose(file);
			}
		}
	}
	lipi_receiver_delete(receiver);
	adc0820_delete(device);
	gpiod_chip_close(chip);
	return 0;
}
