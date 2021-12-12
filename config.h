#ifndef CONFIG_H
#define CONFIG_H

#include <stddef.h>

struct lipi_config {
	unsigned long period;
	unsigned long threshold;
	size_t code_length;
	char start_code;
	char end_code;
};

#endif
