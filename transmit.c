#include "encode.h"
#include "led.h"
#include "transmitter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_CAPACITY 1024

void transmit(struct lipi_transmitter *transmitter,
	      const char *str,
	      size_t count)
{
	size_t length = 2 * count + 2;
	char *buffer = malloc(length);
	encode4b5b(buffer + 1, str, count);
	buffer[0] = transmitter->config->start_code;
	buffer[length - 1] = transmitter->config->end_code;
	lipi_transmit(transmitter, buffer, length);
	free(buffer);
}

void transmit_file(struct lipi_transmitter *transmitter, const char *filename)
{
	FILE *file = fopen(filename, "r");
	if (file == NULL) {
		perror("No such file or directory.");
		printf("Skipping %s.", filename);
		return;
	}
	fseek(file, 0, SEEK_END);
	size_t length = ftell(file);
	fseek(file, 0, SEEK_SET);
	char *buffer = malloc(length);
	fread(buffer, length, 1, file);
	fclose(file);
	transmit(transmitter, buffer, length);
	free(buffer);
}

int main(int argc, char *argv[])
{
	const char chipname[] = "gpiochip0";
	struct gpiod_chip *chip = gpiod_chip_open_by_name(chipname);
	const int pin = 18;
	struct led_device *device = led_new(chip, pin);
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
	struct lipi_transmitter *transmitter =
		lipi_transmitter_new(&config, device, led_write);
	if (argc > 1) {
		for (size_t i = 1; i < argc; ++i)
			transmit_file(transmitter, argv[i]);
	} else {
		for (;;) {
			char buffer[BUFFER_CAPACITY] = { 0 };
			scanf("%s", buffer);
			transmit(transmitter, buffer, strlen(buffer));
		}
	}
	lipi_transmitter_delete(transmitter);
	led_delete(device);
	gpiod_chip_close(chip);
}
