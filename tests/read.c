#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../buf_reader.h"

void on_read_fail(const char* err)
{
	fprintf(stderr, "Failed to read data!\nBufReader error: %s\n", err);
	exit(EXIT_FAILURE);
}

int main(int argc, char** argv)
{
	// Data to be read by BufReader
	uint8_t buffer[] = {
		42,             // byte_value
		0x30, 0x39,        // u16_value (12345)
		0x0f, 0x3d, 0x15, 0x6e, // u32_value (987654)
		'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!', 0 // str_value
	};

	// Create a BufReader from the buffer
	BufReaderCreateOptions options = BR_OPTIONS_FROM_ARRAY(buffer, NULL);
	BufReader* reader = br_create(options);

	// Read data from the buffer
	uint8_t byte_value = br_u8(reader);
	uint16_t u16_value = br_u16(reader);
	uint32_t u32_value = br_u32(reader);
	BufReaderSlice str_slice = br_str(reader);

	// Print the read values
	printf("Read byte: %u\n", byte_value);
	printf("Read uint16_t: %u\n", u16_value);
	printf("Read uint32_t: %u\n", u32_value);
	printf("Read string: %.*s\n", (int)str_slice.size, (char*)str_slice.data);

	// Optional: You can check if the buffer was overrun
	if (br_overran(reader)) {
		printf("Buffer overrun detected!\n");
	}

	// Optional: Destroy the reader when done
	br_destroy(reader);
	return 0;
}