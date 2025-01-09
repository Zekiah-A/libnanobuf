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

#define br_from_writer(writer, fail_callback) \
	br_create((BufReaderCreateOptions) { .fail_cb = (fail_callback), .source_type = BR_SOURCE_BUF_WRITER, .buf_writer = (writer) })

#define br_from_slice(slice, fail_callback) \
	br_create((BufReaderCreateOptions) { .fail_cb = (fail_callback), .source_type = BR_SOURCE_SLICE, .slice = (slice) })

#define br_from_buffer(buffer, fail_callback) \
	br_create((BufReaderCreateOptions) { .fail_cb = (fail_callback), .source_type = BR_SOURCE_BUFFER, .buffer = (buffer) })

#define br_from_ptr(ptr, size, fail_callback) \
	br_create((BufReaderCreateOptions) { .fail_cb = (fail_callback), .source_type = BR_SOURCE_BUFFER, .buffer = &(NanoBufBuffer){ .start = (uint8_t*)(ptr), .end = (uint8_t*)(ptr) + (size) } })

#define br_from_array(array, fail_callback) \
	br_create((BufReaderCreateOptions) { .fail_cb = (fail_callback), .source_type = BR_SOURCE_BUFFER, .buffer = &(NanoBufBuffer){ .start = (uint8_t*)(array), .end = (uint8_t*)(array) + sizeof(array) } })
