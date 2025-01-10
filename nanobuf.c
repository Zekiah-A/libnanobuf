#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nanobuf.h"

void nb_hexdump(const void* arr, size_t n)
{
	const uint8_t* pos = (const uint8_t*) arr;
	const uint8_t* end = pos + n;

	char hex_pair[4] = { 0, 0, ' ', 0 };
	while (true) {
		uint8_t byte = *pos++;
		hex_pair[0] = "0123456789abcdef"[byte >> 4];
		hex_pair[1] = "0123456789abcdef"[byte & 15];
		if (pos == end) {
			hex_pair[2] = '\n';
			fputs(hex_pair, stdout);
			break;
		}
		fputs(hex_pair, stdout);
	}
}

char* nb_to_cstr(BufReaderSlice slice)
{
	char* c_string = (char*)malloc(slice.size + 1);
	if (!c_string) {
		return NULL;
	}
	memcpy(c_string, slice.data, slice.size);
	c_string[slice.size] = '\0';
	return c_string;
}
