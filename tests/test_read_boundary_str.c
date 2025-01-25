#include <stdbool.h>
#include <string.h>

#include "../nanobuf.h"
#include "test_utils.h"

int main(void)
{
	// Length at boundary conditions (max single-byte length)
	uint8_t buffer[64];
	buffer[0] = 63;
	memset(buffer + 1, 'x', 63);
	
	const size_t expected_len = 63;
	char expected_str[64];
	memset(expected_str, 'x', 63);
	expected_str[63] = '\0';

	BufReader reader = br_from_array(buffer, &on_read_fail);
	BufReaderSlice str_slice = br_str(&reader);
	ASSERT(!br_overran(&reader), "BufReader overrun");

	char* cstring = nb_to_cstr(str_slice);
	ASSERT(cstring != NULL, "Cstring was null");
	ASSERT_EQ(expected_len, strlen(cstring), "Length match in cstring");
	ASSERT_MEM_EQ(expected_str, cstring, expected_len, "String content doesn't match");

	return EXIT_SUCCESS;
}
