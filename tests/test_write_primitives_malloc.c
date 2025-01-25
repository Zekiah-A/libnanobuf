#include <stdbool.h>
#include <stdio.h>

#include "../nanobuf.h"
#include "../buf_writer.h"
#include "test_utils.h"

int main(void)
{
	BufWriterCreateOptions options = {
		.fail_cb = &on_write_fail,
		.intial_size = 8
	};
	bw_heapfree(writer) = bw_malloc(bw_create(options));

	uint8_t byte_value = 42;
	uint16_t u16_value = 12345;
	uint32_t u32_value = 987654;
	uint64_t u64_value = 10111213141516171819UL;

	bw_u8(writer, byte_value);
	bw_u16(writer, u16_value);
	bw_u32(writer, u32_value);
	bw_u64(writer, u64_value);

	printf("Buffer size: %zu bytes\n", bw_size(writer));
	nb_hexdump(writer->start, bw_size(writer));

	return 0;
}
