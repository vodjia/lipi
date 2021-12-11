#include "adc0820.h"
#include <stdlib.h>

struct adc0820_device *adc0820_new(struct gpiod_chip *chip,
				   struct adc0820_pinout *pinout)
{
	struct adc0820_device *device = malloc(sizeof(*device));
	device->chip = chip;
	device->db0 = gpiod_chip_get_line(device->chip, pinout->db0);
	device->db1 = gpiod_chip_get_line(device->chip, pinout->db1);
	device->db2 = gpiod_chip_get_line(device->chip, pinout->db2);
	device->db3 = gpiod_chip_get_line(device->chip, pinout->db3);
	device->wr_n = gpiod_chip_get_line(device->chip, pinout->wr_n);
	device->mode = gpiod_chip_get_line(device->chip, pinout->mode);
	device->rd_n = gpiod_chip_get_line(device->chip, pinout->rd_n);
	device->int_n = gpiod_chip_get_line(device->chip, pinout->int_n);
	device->cs_n = gpiod_chip_get_line(device->chip, pinout->cs_n);
	device->db4 = gpiod_chip_get_line(device->chip, pinout->db4);
	device->db5 = gpiod_chip_get_line(device->chip, pinout->db5);
	device->db6 = gpiod_chip_get_line(device->chip, pinout->db6);
	device->db7 = gpiod_chip_get_line(device->chip, pinout->db7);
	device->ofl_n = gpiod_chip_get_line(device->chip, pinout->ofl_n);
	gpiod_line_request_input(device->db0, "ADC0820");
	gpiod_line_request_input(device->db1, "ADC0820");
	gpiod_line_request_input(device->db2, "ADC0820");
	gpiod_line_request_input(device->db3, "ADC0820");
	gpiod_line_request_output(device->wr_n, "ADC0820", 0);
	gpiod_line_request_output(device->mode, "ADC0820", 0);
	gpiod_line_request_output(device->rd_n, "ADC0820", 1);
	gpiod_line_request_input(device->int_n, "ADC0820");
	gpiod_line_request_output(device->cs_n, "ADC0820", 0);
	gpiod_line_request_input(device->db4, "ADC0820");
	gpiod_line_request_input(device->db5, "ADC0820");
	gpiod_line_request_input(device->db6, "ADC0820");
	gpiod_line_request_input(device->db7, "ADC0820");
	gpiod_line_request_input(device->ofl_n, "ADC0820");
	return device;
}

void adc0820_delete(struct adc0820_device *device)
{
	gpiod_line_release(device->db0);
	gpiod_line_release(device->db1);
	gpiod_line_release(device->db2);
	gpiod_line_release(device->db3);
	gpiod_line_release(device->wr_n);
	gpiod_line_release(device->mode);
	gpiod_line_release(device->rd_n);
	gpiod_line_release(device->int_n);
	gpiod_line_release(device->cs_n);
	gpiod_line_release(device->db4);
	gpiod_line_release(device->db5);
	gpiod_line_release(device->db6);
	gpiod_line_release(device->db7);
	gpiod_line_release(device->ofl_n);
	free(device);
}

unsigned char adc0820_read(struct adc0820_device *device)
{
	gpiod_line_set_value(device->cs_n, 0);
	gpiod_line_set_value(device->mode, 0);
	gpiod_line_set_value(device->rd_n, 0);
	while (gpiod_line_get_value(device->int_n))
		continue;
	unsigned char reading = gpiod_line_get_value(device->db0)
		| gpiod_line_get_value(device->db1) << 1
		| gpiod_line_get_value(device->db2) << 2
		| gpiod_line_get_value(device->db3) << 3
		| gpiod_line_get_value(device->db4) << 4
		| gpiod_line_get_value(device->db5) << 5
		| gpiod_line_get_value(device->db6) << 6
		| gpiod_line_get_value(device->db7) << 7;
	gpiod_line_set_value(device->rd_n, 1);
	return reading;
}
