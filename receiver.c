#include "receiver.h"
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>

struct lifi_receiver *lifi_receiver_new(void *device,
					void *read,
					unsigned long period,
					unsigned long threshold,
					size_t code_length,
					char start_code,
					char end_code)
{
	struct lifi_receiver *receiver = malloc(sizeof(*receiver));
	receiver->device = device;
	receiver->read = read;
	receiver->period = period;
	receiver->threshold = threshold;
	receiver->code_length = code_length;
	receiver->start_code = start_code;
	receiver->end_code = end_code;
	return receiver;
}

void lifi_receiver_delete(struct lifi_receiver *receiver)
{
	free(receiver);
}

char lifi_receive_bit(struct lifi_receiver *receiver)
{
	struct timeval current;
	gettimeofday(&current, NULL);
	usleep(receiver->period - current.tv_usec % receiver->period);
	if (receiver->read(receiver->device) > receiver->threshold)
		return 1;
	return 0;
}

char lifi_receive_bits(struct lifi_receiver *receiver, size_t count)
{
	char code = 0;
	for (size_t i = 0; i < count; ++i) {
		code <<= 1;
		code |= lifi_receive_bit(receiver);
	}
	return code;
}

int lifi_receive(struct lifi_receiver *receiver, char *str, size_t count)
{
	size_t i = 0;
	while (i < count) {
		char code = lifi_receive_bits(receiver, receiver->code_length);
		if (code == receiver->end_code)
			return i;
		str[i++] = code;
	}
	return -1;
}
