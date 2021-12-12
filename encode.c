#include "encode.h"
#include <limits.h>
#include <stddef.h>

void encode4b5b(char *dest, const char *src, size_t count)
{
	static char lookup[] = {
		0x1E, 0x09, 0x14, 0x15,
		0x0A, 0x0B, 0x0E, 0x0F,
		0x12, 0x13, 0x16, 0x17,
		0x1A, 0x1B, 0x1C, 0x1D
	};
	for (size_t i = 0; i < count; ++i) {
		dest[2 * i] = lookup[(unsigned)(src[i] >> CHAR_BIT / 2)];
		dest[2 * i + 1] = lookup[(unsigned)src[i] & 0xF];
	}
}
