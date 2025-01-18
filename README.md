# libnanobuf
A super simple, easy, and modern C binary serialisation/deserialisation library
support for variable length integer and string types. Intended for use in high
performance networking scenarios, such as games, where low bandwidth usage, speed
and efficient data transmission is needed.

## Demo
Below is an example demonstrating how to use `BufReader` and `BufWriter` to read and write data.

### Writing Data with `BufWriter`:
```c
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include <nanobuf.h>
#include <buf_writer.h>

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
	const char* cstr_value = "Hello, world!";

	bw_u8(writer, byte_value);
	bw_u16(writer, u16_value);
	bw_u32(writer, u32_value);
	bw_cstr(writer, str_value); // Write the string

	// Print the size of the written data
	printf("Buffer size: %zu bytes\n", bw_size(writer));
	nb_hexdump(writer->start, bw_size(writer));

	// Optional: You can destroy the writer when done
	bw_destroy(writer, true);
	return 0;
}
```

### Reading Data with `BufReader`:
```c
#include <stdio.h>
#include <stdlib.h>

#include <buf_reader.h>

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
		'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!', 0 // cstr_value
	};

	// Create a BufReader from the buffer
	BufReader* reader = br_from_array(buffer, &on_read_fail);

	// Read data from the buffer
	uint8_t byte_value = br_u8(reader);
	uint16_t u16_value = br_u16(reader);
	uint32_t u32_value = br_u32(reader);
	char* cstr_value = br_cstr(reader);

	// Print the read values
	printf("Read byte: %u\n", byte_value);
	printf("Read uint16_t: %u\n", u16_value);
	printf("Read uint32_t: %u\n", u32_value);
	printf("Read string: %.*s\n", cstr_value);

	// Optional: You can check if the buffer was overrun
	if (br_overran(reader)) {
		printf("Buffer overrun detected!\n");
	}

	// Optional: Destroy the reader when done
	br_destroy(reader);
	return 0;
}
```

## Build instructions

To build `libnanobuf`, you can follow these steps:

### Prerequisites:
- CMake 3.31 or higher
- A C23-compliant C compiler

### System installation steps:
1. Clone the repository or download the source files.

   ```bash
   git clone <repository-url>
   cd <repository-folder>
   ```
2. Create a build directory and navigate into it:
    ```bash
    mkdir build
    cd build
    ```
3. Run CMake to generate the build system:
    ```bash
    cmake ..
    ```
4. Build the library:
    ```bash
    make
    ```
5. (Optional) Install the library:
    ```bash
    sudo make install
    ```

This will install the library into the default system directories, typically
`/usr/local/lib` and `/usr/local/include`.

### Submodule installation steps:
1. Add the repository as a submodule.

   ```bash
   cd <your project lib folder>
   git submodule add https://github.com/Zekiah-A/libnanobuf
   ```
2. Add libnanobuf directory to your cmake configuration
    ```cmake
    # CMakeLists.txt
    # Add subdirectory for libnanobuf library
    add_subdirectory(${CMAKE_SOURCE_DIR}/<your project lib folder>/libnanobuf)
    ```
3. Ensure libnanobuf is linked during your buildstep
    ```cmake
    target_link_libraries(${PROJECT_NAME} nanobuf)
    ```

## License
This program is licensed under the permissive [MIT license](./LICENSE).
