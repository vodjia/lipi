#include "led.h"

struct led_device *led_new(struct gpiod_chip *chip, int pin)
{
	struct led_device *device = malloc(sizeof(*device));
	device->chip = chip;
	device->on = gpiod_chip_get_line(device->chip, pin);
	gpiod_line_request_output(device->on, "LED", 1);
	return device;
}

void led_delete(struct led_device *device)
{
	gpiod_line_release(device->on);
	free(device);
}

void led_write(struct led_device *device, char value)
{
	gpiod_line_set_value(device->on, value != 0);
}
