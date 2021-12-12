#include "encode.h"
#include "led.h"
#include "transmitter.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	const char chipname[] = "gpiochip0";
	struct gpiod_chip *chip = gpiod_chip_open_by_name(chipname);
	const int pin = 18;
	struct led_device *device = led_new(chip, pin);
	const unsigned long period = 1000;
	const unsigned long threshold = 120;
	const size_t code_length = 5;
	const char start_code = 0x18;
	const char end_code = 0x0D;
	struct lipi_config config = {
		.period = period,
		.threshold = threshold,
		.code_length = code_length,
		.start_code = start_code,
		.end_code = end_code
	};
	struct lipi_transmitter *transmitter =
		lipi_transmitter_new(&config, device, led_write);
	lipi_transmitter_delete(transmitter);
	led_delete(device);
	gpiod_chip_close(chip);
}
