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
	return (buf->head += 2) > buf->end ? 0 : buf->head[-2] << 8 | buf->head[-1];
}

uint32_t br_u32(BufReader* buf)
{
	return (buf->head += 4) > buf->end
		? 0
		: buf->head[-4] << 24 | buf->head[-3] << 16 | buf->head[-2] << 8 | buf->head[-1];
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

BufReaderSlice br_arr(BufReader* buf, size_t n)
{
	if (buf->head > buf->end - n) {
		return (BufReaderSlice){buf->head, 0};
	}
	uint8_t *s = buf->head;
	buf->head = s + n;
	return (BufReaderSlice){s, n};
}

int br_overran(const BufReader* buf)
{
	return buf->head > buf->end;
}
