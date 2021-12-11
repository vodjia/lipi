#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "adc0820.h"

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
	for (int i = 0; ; ++i) {
		printf("%d\n", adc0820_read(device));
		usleep(1000);
	}
	adc0820_delete(device);
	gpiod_chip_close(chip);
	return 0;
}
