#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include "adc0820.h"

struct lifi_receiver {
	void *device;
	unsigned char (*read)(void *);
	unsigned long period;
	unsigned long threshold;
	size_t code_length;
	char start_code;
	char end_code;
};

struct lifi_receiver *lifi_receiver_new(void *device,
					void *read,
					unsigned long period,
					unsigned long threshold,
					size_t code_length,
					char start_code,
					char end_code)
{
	struct lifi_receiver *receiver = malloc(sizeof(*receiver));
	receiver->device = device;
	receiver->read = read;
	receiver->period = period;
	receiver->threshold = threshold;
	receiver->code_length = code_length;
	receiver->start_code = start_code;
	receiver->end_code = end_code;
	return receiver;
}

void lifi_receiver_delete(struct lifi_receiver *receiver)
{
	free(receiver);
}

char lifi_receive_bit(struct lifi_receiver *receiver)
{
	struct timeval current;
	gettimeofday(&current, NULL);
	usleep(receiver->period - current.tv_usec % receiver->period);
	if (receiver->read(receiver->device) > receiver->threshold)
		return 1;
	return 0;
}

char lifi_receive_bits(struct lifi_receiver *receiver, size_t count)
{
	char code = 0;
	for (size_t i = 0; i < count; ++i) {
		code <<= 1;
		code |= lifi_receive_bit(receiver);
	}
	return code;
}

int lifi_receive(struct lifi_receiver *receiver, char *str, size_t count)
{
	size_t i = 0;
	while (i < count) {
		char code = lifi_receive_bits(receiver, receiver->code_length);
		if (code == receiver->end_code)
			return i;
		str[i++] = code;
	}
	return -1;
}

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
	const unsigned long threshold = 140;
	const size_t code_length = 5;
	const char start_code = 0x18;
	const char end_code = 0x0d;
	struct lifi_receiver *receiver = lifi_receiver_new(device,
							   adc0820_read,
							   period,
							   threshold,
							   code_length,
							   start_code,
							   end_code);
	char buffer[BUFFER_CAPACITY] = { 0 };
	for (;;) {
		char code = lifi_receive_bit(receiver);
		if (code == 1) {
			code <<= code_length - 1;
			code |= lifi_receive_bits(receiver, code_length - 1);
			if (code == receiver->start_code) {
				size_t length = 
					lifi_receive(receiver,
						     buffer,
						     BUFFER_CAPACITY);
				putchar('[');
				for (size_t i = 0; i < length; ++i)
					printf("%x, ", buffer[i]);
				printf("]\n");
			}
		}
	}
	lifi_receiver_delete(receiver);
	adc0820_delete(device);
	gpiod_chip_close(chip);
	return 0;
}
