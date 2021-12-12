#include <stddef.h>

struct lifi_receiver {
	void *device;
	unsigned char (*read)(void *);
	unsigned long period;
	unsigned long threshold;
	size_t code_length;
	char start_code;
	char end_code;
};

struct lifi_receiver *lifi_receiver_new(void *device,
					void *read,
					unsigned long period,
					unsigned long threshold,
					size_t code_length,
					char start_code,
					char end_code);

void lifi_receiver_delete(struct lifi_receiver *receiver);

char lifi_receive_bit(struct lifi_receiver *receiver);

char lifi_receive_bits(struct lifi_receiver *receiver, size_t count);

int lifi_receive(struct lifi_receiver *receiver, char *str, size_t count);
