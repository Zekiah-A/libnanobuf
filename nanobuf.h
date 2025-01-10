#pragma once
#include "buf_reader.h"
#include "buf_writer.h"

// Prints a hexdump of the input buffer to stdout
void nb_hexdump(const void* __s, size_t n);
// Convert a buf reader slice to a null terminated cstring
char* nb_to_cstr(BufReaderSlice slice);
