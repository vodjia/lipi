#include "receiver.h"
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>

struct lipi_receiver *lipi_receiver_new(struct lipi_config *config,
					void *device,
					void *read)
{
	struct lipi_receiver *receiver = malloc(sizeof(*receiver));
	receiver->config = config;
	receiver->device = device;
	receiver->read = read;
	return receiver;
}

void lipi_receiver_delete(struct lipi_receiver *receiver)
{
	free(receiver);
}

char lipi_receive_bit(struct lipi_receiver *receiver)
{
	struct lipi_config *config = receiver->config;
	struct timeval current;
	gettimeofday(&current, NULL);
	usleep(config->period - current.tv_usec % config->period);
	if (receiver->read(receiver->device) > config->threshold)
		return 1;
	return 0;
}

char lipi_receive_bits(struct lipi_receiver *receiver, size_t count)
{
	char code = 0;
	for (size_t i = 0; i < count; ++i) {
		code <<= 1;
		code |= lipi_receive_bit(receiver);
	}
	return code;
}

int lipi_receive(struct lipi_receiver *receiver, char *str, size_t count)
{
	struct lipi_config *config = receiver->config;
	size_t i = 0;
	while (i < count) {
		char code = lipi_receive_bits(receiver, config->code_length);
		if (code == config->end_code)
			return i;
		str[i++] = code;
	}
	return -1;
}
