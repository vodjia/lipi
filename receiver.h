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
