#pragma once
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "nanobuf_defs.h"

// Default buf writer fail function
void bw_fail(const char* msg);
// Create a new buf writer
BufWriter bw_create(BufWriterCreateOptions options);
// Destroy the underlying buffer of a buf writer
void bw_dispose(BufWriter* buf);
// Destroy a heap alllocated buf writer (including the underlying buffer)
void bw_free(BufWriter** buf);
// Get the current size of data written in the buffer
size_t bw_size(const BufWriter* buf);
// Get the total capacity of the buffer
size_t bw_capacity(const BufWriter* buf);
// Clear the buffer and reset the write position
void bw_clear(BufWriter* buf);
// Expand the buffer to accommodate additional data
void bw_expand(BufWriter* buf);
// Skip a number of bytes in the buffer
void bw_skip(BufWriter* buf, size_t n);
// Write a single byte to the buffer
void bw_u8(BufWriter* buf, uint8_t n);
// Write an unsigned 16-bit integer to the buffer
void bw_u16(BufWriter* buf, uint16_t n);
// Write an unsigned 32-bit integer to the buffer
void bw_u32(BufWriter* buf, uint32_t n);
// Write an unsigned 64-bit integer to the buffer
void bw_u64(BufWriter* buf, uint64_t n);
// Write an array of uint8_t to the buffer
void bw_arr(BufWriter* buf, const uint8_t* arr, size_t n);
// Write a variable length string to the buffer
void bw_str(BufWriter* buf, const char* str);
// Write a null-terminated C string to the buffer
void bw_cstr(BufWriter* buf, const char* cstr);