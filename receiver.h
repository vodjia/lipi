#include "config.h"
#include <stddef.h>

struct lipi_receiver {
	struct lipi_config *config;
	void *device;
	unsigned char (*read)(void *);
};

struct lipi_receiver *lipi_receiver_new(struct lipi_config *config,
					void *device,
					void *read);

void lipi_receiver_delete(struct lipi_receiver *receiver);

char lipi_receive_bit(struct lipi_receiver *receiver);

char lipi_receive_bits(struct lipi_receiver *receiver, size_t count);

int lipi_receive(struct lipi_receiver *receiver, char *str, size_t count);
