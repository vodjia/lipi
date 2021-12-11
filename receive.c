#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "adc0820.h"

struct lifi_receiver {
	void *device;
	unsigned char (*read)(void *);
	unsigned long period;
	unsigned long threshold;
	unsigned char start_pattern;
	unsigned char end_pattern;
};

struct lifi_receiver *lifi_receiver_new(void *device,
					void *read,
					unsigned long period,
					unsigned long threshold,
					unsigned char start_pattern,
					unsigned char end_pattern)
{
	struct lifi_receiver *receiver = malloc(sizeof(*receiver));
	receiver->device = device;
	receiver->read = read;
	receiver->period = period;
	receiver->threshold = threshold;
	receiver->start_pattern = start_pattern;
	receiver->end_pattern = end_pattern;
	return receiver;
}

void lifi_receiver_delete(struct lifi_receiver *receiver)
{
	free(receiver);
}

unsigned char lifi_receive_bit(struct lifi_receiver *receiver)
{
	if (receiver->read(receiver->device) > receiver->threshold)
		return 1;
	return 0;
}

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
	const unsigned char start_pattern = 0x18;
	const unsigned char end_pattern = 0x0d;
	struct lifi_receiver *receiver = lifi_receiver_new(device,
							   adc0820_read,
							   period,
							   threshold,
							   start_pattern,
							   end_pattern);
	for (int i = 0; ; ++i) {
		printf("%d\n", lifi_receive_bit(receiver));
		usleep(1000);
	}
	lifi_receiver_delete(receiver);
	adc0820_delete(device);
	gpiod_chip_close(chip);
	return 0;
}
