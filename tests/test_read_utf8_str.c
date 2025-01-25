#include <stdbool.h>
#include <string.h>

#include "../nanobuf.h"
#include "test_utils.h"

int main(void)
{
	// UTF-8 string ("helló")
	uint8_t buffer[] = {6, 'h', 'e', 'l', 'l', 0xc3, 0xb3};
	const char* expected_str = "helló";

	BufReader reader = br_from_array(buffer, &on_read_fail);
	BufReaderSlice str_slice = br_str(reader);
	ASSERT(!br_overran(reader), "BufReader overrun");

	char* cstring = nb_to_cstr(str_slice);
	ASSERT(cstring != NULL, "Cstring was null");
	ASSERT_EQ(strlen(expected_str), strlen(cstring), "Length match in cstring");
	ASSERT_MEM_EQ(expected_str, cstring, strlen(expected_str), "String content doesn't match");

	return EXIT_SUCCESS;
}
