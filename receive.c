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
	const unsigned long period = 8000;
	const unsigned long threshold = 100;
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
#ifndef NDEBUG
	size_t total_count = 0;
	size_t error_count = 0;
#endif
	for (;;) {
		char code = lipi_receive_bit(receiver);
		if (code == start_code >> (code_length - 1)) {
			code <<= code_length - 1;
			code |= lipi_receive_bits(receiver, code_length - 1);
			if (code == start_code) {
				++total_count;
				char raw[BUFFER_CAPACITY] = { 0 };
				size_t length = 
					lipi_receive(receiver,
						     raw,
						     BUFFER_CAPACITY);
				char data[BUFFER_CAPACITY] = { 0 };
				if (decode4b5b(data, raw) < 0) {
#ifndef NDEBUG
					++error_count;
#endif
				}
#ifdef NDEBUG
				printf("%s\n", data);
#endif
				FILE *file = fopen("received.txt", "w");
				if (fputs(data, file) == EOF) {
					perror("Failed to write.");
					return -1;
				}
				fclose(file);
#ifndef NDEBUG
				printf("________________\n");
				printf("Total count: %d\n", total_count);
				printf("Error count: %d\n", error_count);
				printf("Error rate: %lf%%\n", (double)error_count / total_count * 100);
#endif
			}
		}
	}
	lipi_receiver_delete(receiver);
	adc0820_delete(device);
	gpiod_chip_close(chip);
	return 0;
}
