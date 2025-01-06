#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "../nanobuf.h"
#include "../buf_writer.h"

void on_write_fail(const char* err)
{
	fprintf(stderr, "Failed to write data!\nBufWriter error: %s\n", err);
	exit(EXIT_FAILURE);
}

int main(int argc, char** argv)
{
	// Create a BufWriter with an initial size of 1024 bytes
	BufWriterCreateOptions options = {
		.fail_cb = &on_write_fail,   // A fail callback function (set to NULL for default)
		.intial_size = 1024   // Initial buffer size (bytes)
	};
	BufWriter* writer = bw_create(options);

	// Write various types of data to the buffer
	uint8_t byte_value = 42;
	uint16_t u16_value = 12345;
	uint32_t u32_value = 987654;
	const char* str_value = "Hello, world!";

	bw_u8(writer, byte_value);
	bw_u16(writer, u16_value);
	bw_u32(writer, u32_value);
	bw_str(writer, str_value); // Write the string

	// Print the size of the written data
	printf("Buffer size: %zu bytes\n", bw_size(writer));
	nb_hexdump(writer->start, bw_size(writer));

	// Optional: You can destroy the writer when done
	bw_destroy(writer, true);
	return 0;
}
