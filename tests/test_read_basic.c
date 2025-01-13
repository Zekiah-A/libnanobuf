#include <stdlib.h>
#include "../buf_reader.h"
#include "test_utils.h"

int main(void)
{
	uint8_t buffer[] = {
		42,                                 // byte_value (42)
		0x30, 0x39,                     // u16_value (12345)
		0x00, 0x0f, 0x12, 0x06  // u32_value (987654)
	};

	BufReader* reader = br_from_array(buffer, &on_read_fail);
	
	uint8_t byte_value = br_u8(reader);
	ASSERT(!br_overran(reader), "BufReader overrun");
	ASSERT_EQ(42, byte_value, "byte_value");

	uint16_t u16_value = br_u16(reader);
	ASSERT(!br_overran(reader), "BufReader overrun");
	ASSERT_EQ(12345, u16_value, "u16_value");

	uint32_t u32_value = br_u32(reader);
	ASSERT(!br_overran(reader), "BufReader overrun");
	ASSERT_EQ(987654, u32_value, "u32_value");

	br_destroy(reader);
	return EXIT_SUCCESS;
}
