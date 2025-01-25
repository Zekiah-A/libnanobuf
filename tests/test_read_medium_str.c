#include <stdbool.h>
#include <string.h>

#include "../nanobuf.h"
#include "test_utils.h"

int main(void)
{
	// Medium string (0x3F < length < 0x3FFF)
	// Creating a 100-byte string (0x64 bytes, requires 2-byte length encoding)
	uint8_t buffer[102];                     // 2 bytes for length + 100 bytes data
	buffer[0] = 0x40 | (100 >> 8);           // First length byte with bit 6 set
	buffer[1] = 100 & 0xFF;                  // Second length byte
	memset(buffer + 2, 'a', 100);    // Fill with 'a's

	const size_t expected_len = 100;
	char expected_str[101];
	memset(expected_str, 'a', 100);
	expected_str[100] = '\0';

	BufReader reader = br_from_array(buffer, &on_read_fail);
	BufReaderSlice str_slice = br_str(&reader);
	ASSERT(!br_overran(&reader), "BufReader overrun");

	char* cstring = nb_to_cstr(str_slice);
	ASSERT(cstring != NULL, "Cstring was null");
	ASSERT_EQ(expected_len, strlen(cstring), "Length match in cstring");
	ASSERT_MEM_EQ(expected_str, cstring, expected_len, "String content doesn't match");

	return EXIT_SUCCESS;
}
