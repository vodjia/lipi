#include "encode.h"
#include "led.h"
#include "transmitter.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	if (argc < 2) {
		puts("No input files.");
		return -1;
	}
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
	for (size_t i = 1; i < argc; ++i) {
		FILE *file = fopen(argv[i], "r");
		if (file == NULL) {
			perror("No such file or directory.");
			printf("Skipping %s.", argv[i]);
			continue;
		}
		fseek(file, 0, SEEK_END);
		size_t file_length = ftell(file);
		fseek(file, 0, SEEK_SET);
		size_t message_length = file_length;
		char *message = malloc(message_length + 1);
		fread(message, file_length, 1, file);
		fclose(file);
		message[message_length] = '\0';
		size_t encoded_length = 2 * message_length + 2;
		char *encoded = malloc(encoded_length + 1);
		encode4b5b(encoded + 1, message);
		free(message);
		encoded[0] = start_code;
		encoded[encoded_length - 1] = end_code;
		encoded[encoded_length] = '\0';
		lipi_transmit(transmitter, encoded);
		free(encoded);
	}
	lipi_transmitter_delete(transmitter);
	led_delete(device);
	gpiod_chip_close(chip);
}
