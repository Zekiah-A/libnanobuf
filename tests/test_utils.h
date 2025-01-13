#pragma once
#include <stdio.h>
#include <stdlib.h>

#define ASSERT(condition, message) \
	do { \
		if (!(condition)) { \
			fprintf(stderr, "Assertion failed: %s\n", message); \
			return EXIT_FAILURE; \
		} \
	} while (0)

#define ASSERT_EQ(expected, actual, message) \
	do { \
		if ((expected) != (actual)) { \
			fprintf(stderr, "%s: expected %d, got %d\n", \
					message, (int)(expected), (int)(actual)); \
			return EXIT_FAILURE; \
		} \
	} while (0)
#define ASSERT_MEM_EQ(expected, actual, length, message) \
	do { \
		if (memcmp((expected), (actual), (length)) != 0) { \
			fprintf(stderr, "Memory comparison failed: %s\n", message); \
			fprintf(stderr, "Expected: '"); \
			for (size_t i = 0; i < (length); i++) { \
				fprintf(stderr, "%c", ((const char*)(expected))[i]); \
			} \
			fprintf(stderr, "'\nGot:      '"); \
			for (size_t i = 0; i < (length); i++) { \
				fprintf(stderr, "%c", ((const char*)(actual))[i]); \
			} \
			fprintf(stderr, "'\n"); \
			return EXIT_FAILURE; \
		} \
	} while (0)

void on_read_fail(const char* err)
{
	fprintf(stderr, "Failed to read data!\nBufReader error: %s\n", err);
	exit(EXIT_FAILURE);
}


void on_write_fail(const char* err)
{
	fprintf(stderr, "Failed to write data!\nBufWriter error: %s\n", err);
	exit(EXIT_FAILURE);
}

