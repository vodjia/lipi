#ifndef TRANSMITTER_H
#define TRANSMITTER_H

#include "config.h"

struct lipi_transmitter {
	struct lipi_config *config;
	void *device;
	void (*write)(void *, char value);
};

struct lipi_transmitter *lipi_transmitter_new(struct lipi_config *config,
					      void *device,
					      void *write);

void lipi_transmitter_delete(struct lipi_transmitter *transmitter);

void lipi_transmit_bit(struct lipi_transmitter *transmitter, char bit);

void lipi_transmit(struct lipi_transmitter *transmitter,
		   char *str,
		   size_t count);

#endif
