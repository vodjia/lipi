#ifndef LED_H
#define LED_H

#include <gpiod.h>

struct led_device {
	struct gpiod_chip *chip;
	struct gpiod_line *on;
};

struct led_device *led_new(struct gpiod_chip *chip, int pin);

void led_delete(struct led_device *device);

void led_write(struct led_device *device, char value);

#endif
