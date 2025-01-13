#include <stdbool.h>
#include <string.h>

#include "../nanobuf.h"
#include "test_utils.h"

int main(void)
{
	// Short string (length < 0x3F)
	uint8_t buffer[] = { 5, 'h', 'e', 'l', 'l', 'o' };

	const size_t expected_len = 5;
	const char* expected_str = "hello";

	BufReader* reader = br_from_array(buffer, &on_read_fail);
	BufReaderSlice str_slice = br_str(reader);
	ASSERT(!br_overran(reader), "BufReader overrun");

	char* cstring = nb_to_cstr(str_slice);
	ASSERT(cstring != NULL, "Cstring was null");
	ASSERT_EQ(expected_len, strlen(cstring), "Length match in cstring");
	ASSERT_MEM_EQ(expected_str, cstring, expected_len, "String content doesn't match");

	br_destroy(reader);
	return EXIT_SUCCESS;
}
