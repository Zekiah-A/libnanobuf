#include <stdbool.h>
#include <string.h>

#include "../nanobuf.h"
#include "test_utils.h"

int main(void)
{
	// Short string (length < 0x3F)
	uint8_t buffer[] = { 'h', 'e', 'l', 'l', 'o', '\0' };

	const size_t expected_len = 5;
	const char* expected_str = "hello";

	BufReader reader = br_from_array(buffer, &on_read_fail);
	char* cstr = br_cstr(&reader);
	ASSERT(!br_overran(&reader), "BufReader overrun");

	ASSERT(cstr != NULL, "Cstring was null");
	ASSERT_EQ(expected_len, strlen(cstr), "Length match in cstring");
	ASSERT_MEM_EQ(expected_str, cstr, expected_len, "String content doesn't match");

	free(cstr);
	return EXIT_SUCCESS;
}
