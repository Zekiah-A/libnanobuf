#pragma once
#include <stddef.h>
#include <stdint.h>

#include "nanobuf_defs.h"

// Default buf reader fail function
void br_fail(const char* msg);
// Initialise a buffer reader
BufReader br_create(BufReaderCreateOptions options);
// Return remaining bytes in the buffer
size_t br_remaining(const BufReader* buf);
// Skip a number of bytes in the buffer
void br_skip(BufReader* buf, size_t n);
// Read a single byte with fallback
uint8_t br_u8(BufReader* buf);
// Peek a byte without advancing the buffer
uint8_t br_peek(const BufReader* buf, size_t n);
// Read an unsigned 16-bit integer
uint16_t br_u16(BufReader* buf);
// Read an unsigned 32-bit integer
uint32_t br_u32(BufReader* buf);
// Read an unsigned 64-bit integer
uint64_t br_u64(BufReader* buf);
// Read a signed 64-bit variable-length integer
uint64_t br_v64(BufReader* buf);
// Read an array of uint8_t
BufReaderSlice br_arr(BufReader* buf);
// Read a variable length string slice
BufReaderSlice br_str(BufReader* buf);
// Read a null-terminated C string
char* br_cstr(BufReader* buf);
// Check if the buffer has been overrun
int br_overran(const BufReader* buf);
