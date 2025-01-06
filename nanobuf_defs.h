#pragma once
#include <stddef.h>
#include <stdint.h>

/// Nanobuf
typedef struct nb_buffer {
	uint8_t* start;
	uint8_t* end;
} NanoBufBuffer;

/// Buf writer

typedef void (*BufWriterFailCallback)(const char*);

typedef struct bw_buffer {
	BufWriterFailCallback fail_cb;
	uint8_t* start;
	uint8_t* end;
	uint8_t* head;
} BufWriter;

typedef struct bw_create_options {
	BufWriterFailCallback fail_cb;
	size_t intial_size;
} BufWriterCreateOptions;

/// Buf reader

typedef void (*BufReaderFailCallback)(const char*);

typedef struct br_buffer {
	BufReaderFailCallback fail_cb;
	uint8_t* head;
	uint8_t* end;
} BufReader;

typedef struct br_slice {
	void* data;
	size_t size;
} BufReaderSlice;

typedef enum br_source_type {
    BR_SOURCE_BUF_WRITER,
    BR_SOURCE_SLICE,
    BR_SOURCE_BUFFER
} BufReaderSourceType;

typedef struct br_create_options {
	BufReaderFailCallback fail_cb;
	BufReaderSourceType source_type;
	union {
		const BufWriter* buf_writer;
		const BufReaderSlice* slice;
		const NanoBufBuffer* buffer;
	};
} BufReaderCreateOptions;

#define BR_OPTIONS_FROM_WRITER(writer, fail_callback) \
	(BufReaderCreateOptions) { .fail_cb = (fail_callback), .source_type = BR_SOURCE_BUF_WRITER, .buf_writer = (writer) }

#define BR_OPTIONS_FROM_SLICE(slice, fail_callback) \
	(BufReaderCreateOptions) { .fail_cb = (fail_callback), .source_type = BR_SOURCE_SLICE, .slice = (slice) }

#define BR_OPTIONS_FROM_BUFFER(buffer, fail_callback) \
	(BufReaderCreateOptions) { .fail_cb = (fail_callback), .source_type = BR_SOURCE_BUFFER, .buffer = (buffer) }

#define BR_OPTIONS_FROM_DATA(ptr, size, fail_callback) \
	(BufReaderCreateOptions) { .fail_cb = (fail_callback), .source_type = BR_SOURCE_BUFFER, .buffer = &(NanoBufBuffer){ .start = (uint8_t*)(ptr), .end = (uint8_t*)(ptr) + (size) } }

#define BR_OPTIONS_FROM_ARRAY(array, fail_callback) \
	(BufReaderCreateOptions) { .fail_cb = (fail_callback), .source_type = BR_SOURCE_BUFFER, .buffer = &(NanoBufBuffer){ .start = (uint8_t*)(array), .end = (uint8_t*)(array) + sizeof(array) } }
