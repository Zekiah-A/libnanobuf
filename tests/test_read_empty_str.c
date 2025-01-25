#include <stdbool.h>
#include <string.h>

#include "../nanobuf.h"
#include "test_utils.h"

int main(void)
{
	// Empty string should be encoded as [0]
	uint8_t buffer[] = { 0 };

	BufReader reader = br_from_array(buffer, &on_read_fail);
	BufReaderSlice str_slice = br_str(&reader);
	ASSERT(!br_overran(&reader), "BufReader overrun");

	char* cstring = nb_to_cstr(str_slice);
	ASSERT(cstring != NULL, "Cstring was null");
	ASSERT_EQ(0, strlen(cstring), "Length match in cstring");

	return EXIT_SUCCESS;
}
