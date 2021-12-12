#include "transmitter.h"
#include <sys/time.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>

struct lipi_transmitter *lipi_transmitter_new(struct lipi_config *config,
					      void *device,
					      void *write)
{
	struct lipi_transmitter *transmitter = malloc(sizeof(*transmitter));
	transmitter->config = config;
	transmitter->device = device;
	transmitter->write = write;
	return transmitter;
}

void lipi_transmitter_delete(struct lipi_transmitter *transmitter)
{
	free(transmitter);
}

void lipi_transmit_bit(struct lipi_transmitter *transmitter, char bit)
{
	struct lipi_config *config = transmitter->config;
	struct timeval current;
	gettimeofday(&current, NULL);
	usleep(config->period - current.tv_usec % config->period);
	transmitter->write(transmitter->device, bit);
}

void lipi_transmit(struct lipi_transmitter *transmitter, char *str)
{
	for (size_t i = 0; str[i] != '\0'; ++i) {
		for (size_t j = 0; j < CHAR_BIT; ++j) {
			char bit = str[j] >> (CHAR_BIT - j - 1);
			lipi_transmit_bit(transmitter, bit);
		}
	}
}
