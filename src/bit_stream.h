#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
typedef unsigned char byte_t;

#define min(a, b) (a > b? b:a)
#define max(a, b) (a < b? b:a)

struct bit_stream_t
{
	uint32_t capacity;
	uint32_t pos;
	byte_t * data;
};

typedef struct bit_stream_t bit_stream_t;

void bit_stream_resize(bit_stream_t * stream, uint32_t new_size);

void bit_stream_push_bits(bit_stream_t * stream, byte_t bits, uint32_t count);

void bit_stream_dump(bit_stream_t * stream);

