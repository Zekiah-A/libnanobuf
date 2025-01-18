#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "buf_reader.h"

void br_fail(const char* msg)
{
	fprintf(stderr, "BufReader error: %s\n", msg);
	exit(EXIT_FAILURE);
}

BufReader* br_create(BufReaderCreateOptions options)
{
	BufReaderFailCallback fail_cb = options.fail_cb ? options.fail_cb : &br_fail;

	BufReader* buf = (BufReader*) malloc(sizeof(BufReader));
	if (buf == NULL) {
		fail_cb("Failed to allocate memory for buffer");	
		return NULL;	
	}
	buf->fail_cb = fail_cb;

	switch (options.source_type) {
		case BR_SOURCE_BUF_WRITER: {
			buf->head = options.buf_writer->head;
			buf->end = options.buf_writer->end;
			break;
		}
		case BR_SOURCE_SLICE: {
			buf->head = options.slice->data;
			buf->end = (uint8_t*)options.slice->data + options.slice->size;
			break;
		}
		case BR_SOURCE_BUFFER: {
			buf->head = options.buffer->start;
			buf->end = options.buffer->end;
			break;
		}
		default: {
			fail_cb("Unrecognised buffer source type");
			return NULL;
			break;
		}
	}
	return buf;
}

void br_destroy(BufReader* buf)
{
	if (!buf) {
		return;
	}

	free(buf);
}

size_t br_remaining(const BufReader* buf)
{
	return (size_t)(buf->end - buf->head);
}

void br_skip(BufReader* buf, size_t n)
{
	buf->head += n;
}

uint8_t br_u8(BufReader* buf)
{
	return ++buf->head > buf->end ? 0 : buf->head[-1];
}

uint8_t br_peek(const BufReader* buf, size_t n)
{
	return buf->head >= buf->end - n ? 0 : buf->head[n];
}

uint16_t br_u16(BufReader* buf)
{
	if (buf->head + 2 > buf->end) {
		buf->fail_cb("Buffer overrun while reading u16");
		return 0;
	}

	uint16_t result = (buf->head[0] << 8) | buf->head[1];
	buf->head += 2;
	return result;
}

uint32_t br_u32(BufReader* buf)
{
	if (buf->head + 4 > buf->end) {
		buf->fail_cb("Buffer overrun while reading u32");
		return 0;
	}

	uint32_t result = (buf->head[0] << 24) |
		(buf->head[1] << 16) |
		(buf->head[2] << 8) |
		buf->head[3];
	buf->head += 4;
	return result;
}

uint64_t br_u64(BufReader* buf)
{
	if (buf->head + 8 > buf->end) {
		buf->fail_cb("Buffer overrun while reading u64");
		return 0;
	}

    uint64_t result = ((uint64_t)buf->head[0] << 56) |
		((uint64_t)buf->head[1] << 48) |
		((uint64_t)buf->head[2] << 40) |
		((uint64_t)buf->head[3] << 32) |
		((uint64_t)buf->head[4] << 24) |
		((uint64_t)buf->head[5] << 16) |
		((uint64_t)buf->head[6] << 8) |
		(uint64_t)buf->head[7];
	buf->head += 8;
	return result;
}

uint64_t br_v64(BufReader* buf)
{
	if (buf->head >= buf->end) {
		return (buf->head++, 0);
	}
	uint64_t n = *buf->head++;
	if (n < 64) {
		return n;
	}
	else if (n < 96) {
		return ++buf->head > buf->end ? 0 : (n & 0x1F) << 8 | buf->head[-1];
	}
	else if (n < 128) {
		return (buf->head += 3) > buf->end
			? 0
			: (n & 0x1F) << 24 | buf->head[-3] << 16 | buf->head[-2] << 8 | buf->head[-1];
	}
	else {
		return (buf->head += 7) > buf->end
			? 0
			: (n & 0x7F) << 56 | (uint64_t)buf->head[-7] << 48 |
				(uint64_t)buf->head[-6] << 40 | (uint64_t)buf->head[-5] << 32 |
				buf->head[-4] << 24 | buf->head[-3] << 16 | buf->head[-2] << 8 | buf->head[-1];
	}
}

BufReaderSlice br_arr(BufReader* buf)
{
	if (buf->head >= buf->end) {
		buf->fail_cb("Buffer overrun while reading array");
		return (BufReaderSlice){NULL, 0};
	}

	// Read the array size
	uint32_t n = *buf->head++;
	if (n >= 64) {
		if (n < 128) {
			if (buf->head >= buf->end) {
				buf->fail_cb("Buffer overrun while reading 2-byte array size");
				return (BufReaderSlice){NULL, 0};
			}
			n = ((n & 0x3F) << 8) | *buf->head++;
		} else {
			if (buf->head + 3 > buf->end) {
				buf->fail_cb("Buffer overrun while reading 4-byte array size");
				return (BufReaderSlice){NULL, 0};
			}
			n = ((n & 0x7F) << 24) |
				(buf->head[0] << 16) |
				(buf->head[1] << 8) |
				buf->head[2];
			buf->head += 3;
		}
	}

	// Check if the array size overrun
	if (buf->head + n > buf->end) {
		buf->fail_cb("Buffer overrun while reading array data");
		return (BufReaderSlice){NULL, 0};
	}

	// Create a slice for the array
	uint8_t* start = buf->head;
	buf->head += n;

	return (BufReaderSlice){ start, n };
}


BufReaderSlice br_str(BufReader* buf)
{
	if (buf->head >= buf->end) {
		return (BufReaderSlice) { ++buf->head, 0 };
	}
	uint32_t n = *buf->head++;
	
	// Empty string
	if (n == 0) {
		return (BufReaderSlice) { buf->head, 0 };
	}

	if (n >= 64) {
		if (n < 128) {
			// Two byte length
			if (buf->head >= buf->end) {
				return (BufReaderSlice) {buf->head, 0};
			}
			n = ((n & 0x3F) << 8) | *buf->head++;
		}
		else {
			// Four byte length
			if (buf->head + 3 > buf->end) {
				return (BufReaderSlice) {buf->head, 0};
			}
			// Read big-endian 4-byte integer
			n = ((n & 0x7F) << 24) |
				(buf->head[0] << 16) |
				(buf->head[1] << 8) |
				buf->head[2];
			buf->head += 3;
		}
	}

	// Enough bytes left to read string
	if (buf->head + n > buf->end) {
		return (BufReaderSlice) {buf->head, 0};
	}

	uint8_t* str = buf->head;
	buf->head = str + n;
	return (BufReaderSlice) {str, n};
}

char* br_cstr(BufReader* buf)
{
	if (buf->head >= buf->end) {
		buf->fail_cb("Buffer overrun while reading C string");
		return NULL;
	}

	uint8_t* start = buf->head;
	while (buf->head < buf->end && *buf->head != '\0') {
		buf->head++;
	}

	if (buf->head == buf->end) {
		buf->fail_cb("Null terminator not found in buffer");
		return NULL;
	}

	// Move past null terminator
	buf->head++;

	return strdup((char*) start);
}

int br_overran(const BufReader* buf)
{
	return buf->head > buf->end;
}
