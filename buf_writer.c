#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "buf_writer.h"

void bw_fail(const char* msg)
{
	fprintf(stderr, "BufWriter error: %s\n", msg);
	exit(EXIT_FAILURE);
}

BufWriter* bw_create(BufWriterCreateOptions options)
{
	BufWriterFailCallback fail_cb = options.fail_cb ? options.fail_cb : &bw_fail;
	BufWriter* buf = (BufWriter*) malloc(sizeof(BufWriter));
	if (buf == NULL) {
		fail_cb("Failed to allocate memory for buffer");
	}
	buf->fail_cb = fail_cb;

	size_t malloc_size = options.intial_size >= 0 ? 16 : options.intial_size;
	buf->start = buf->head = (uint8_t*) malloc(malloc_size);
	if (!buf->start) {
		buf->fail_cb("Failed to allocate initial buffer memory");
	}
	buf->end = buf->start + malloc_size;
	return buf;
}

void bw_destroy(BufWriter* buf, bool free_buffer)
{
	if (!buf) {
		return;
	}
	if (free_buffer) {
		free(buf->start);
	}
	free(buf);
}

size_t bw_size(const BufWriter* buf)
{
	return (size_t)(buf->head - buf->start);
}

size_t bw_capacity(const BufWriter* buf)
{
	return (size_t)(buf->end - buf->start);
}

void bw_clear(BufWriter* buf)
{
	if (!buf) {
		return;
	}
	free(buf->start);
	buf->start = buf->head = (uint8_t*)malloc(16);
	if (!buf->start) {
		buf->fail_cb("Failed to reallocate buffer memory");
	}
	buf->end = buf->start + 16;
}

void bw_expand(BufWriter* buf)
{
	if (!buf) {
		return;
	}
	size_t current_size = bw_size(buf);
	size_t new_capacity = bw_capacity(buf) << 1;
	uint8_t *new_start = (uint8_t*)realloc(buf->start, new_capacity);
	if (!new_start) {
		buf->fail_cb("Out of memory while reallocating buffer");
	}
	buf->start = new_start;
	buf->head = buf->start + current_size;
	buf->end = buf->start + new_capacity;
}

void bw_realloc(BufWriter* buf, size_t extra_space)
{
	if (!buf) {
		return;
	}
    size_t current_size = bw_size(buf);
    size_t new_capacity = bw_capacity(buf) * 2 + extra_space;
    uint8_t *new_start = (uint8_t*)realloc(buf->start, new_capacity);
    if (!new_start) {
        buf->fail_cb("Out of memory while reallocating buffer");
    }
    buf->start = new_start;
    buf->head = buf->start + current_size;
    buf->end = buf->start + new_capacity;
}

void bw_skip(BufWriter* buf, size_t n)
{
	if (!buf) {
		return;
	}
	if (buf->head > buf->end - n) {
		size_t current_size = bw_size(buf);
		size_t new_capacity = bw_capacity(buf) * 2 + n;
		uint8_t* new_start = (uint8_t*)realloc(buf->start, new_capacity);
		if (!new_start) {
			buf->fail_cb("Out of memory while reallocating buffer");
		}
		buf->start = new_start;
		buf->head = buf->start + current_size + n;
		buf->end = buf->start + new_capacity;
	}
	else {
		buf->head += n;
	}
}

void bw_u8(BufWriter* buf, uint8_t n)
{
	if (buf->head == buf->end) {
		bw_expand(buf);
	}

	*buf->head++ = n;
}

void bw_u16(BufWriter* buf, uint16_t n)
{
	if (buf->head >= buf->end - 1) {
		bw_expand(buf);
	}

	*buf->head++ = n >> 8;
	*buf->head++ = n;
}

void bw_u32(BufWriter* buf, uint32_t n)
{
	if (buf->head > buf->end - 4) {
		bw_expand(buf);
	}

	*buf->head++ = n >> 24;
	*buf->head++ = n >> 16;
	*buf->head++ = n >> 8;
	*buf->head++ = n;
}

void bw_u64(BufWriter* buf, uint64_t n)
{
	if (buf->head > buf->end - 8) {
		bw_expand(buf);
	}

	*buf->head++ = n >> 56;
	*buf->head++ = n >> 48;
	*buf->head++ = n >> 40;
	*buf->head++ = n >> 32;
	*buf->head++ = n >> 24;
	*buf->head++ = n >> 16;
	*buf->head++ = n >> 8;
	*buf->head++ = n;
}

void bw_arr(BufWriter* buf, const uint8_t* arr, size_t n)
{
	if (buf->head > buf->end - n - 4) {
		size_t current_size = bw_size(buf);
		size_t new_capacity = bw_capacity(buf) * 2 + n;
		uint8_t *new_start = (uint8_t*)realloc(buf->start, new_capacity);
		if (!new_start) {
			buf->fail_cb("Out of memory while reallocating buffer");
		}
		buf->start = new_start;
		buf->head = buf->start + current_size;
		buf->end = buf->start + new_capacity;
	}
	if (n < 64) {
		*buf->head++ = n;
	}
	else if (n < 16384) {
		*buf->head++ = (n >> 8) | 0x40;
		*buf->head++ = n;
	}
	else {
		*buf->head++ = (n >> 24) | 0x80;
		*buf->head++ = n >> 16;
		*buf->head++ = n >> 8;
		*buf->head++ = n;
	}
	if (n) {
		memcpy(buf->head, arr, n);
		buf->head += n;
	}
}

void bw_str(BufWriter* buf, const char* str)
{
	size_t length = strlen(str);
	bw_arr(buf, (const uint8_t*) str, length);
}

void bw_cstr(BufWriter* buf, const char* cstr)
{
	size_t length = strlen(cstr) + 1;
	if (buf->head > buf->end - length) {
		bw_realloc(buf, length);
	}

	memcpy(buf->head, cstr, length);
	buf->head += length;
}