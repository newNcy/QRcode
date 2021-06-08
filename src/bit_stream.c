#include "bit_stream.h"
#include <stdio.h>


void bit_stream_resize(bit_stream_t * stream, uint32_t new_size)
{
	if (stream->capacity == new_size || new_size < 0) {
		return;
	}
	if (new_size%8 != 0) {
		new_size += 8-(new_size%8);
	}
	if (new_size > 0) {
		byte_t * old_data = stream->data;
		stream->data = (byte_t*)malloc(new_size/8);
		memset(stream->data,0, new_size/8);
		memcpy(stream->data, old_data, min(new_size/8, stream->pos/8));
		stream->pos = min(new_size, stream->pos);
		stream->capacity = new_size;
	} else if (stream->capacity > 0) {
		free(stream->data);
		stream->pos = 0;
		stream->capacity = 0;
	}
}

void bit_stream_push_bits(bit_stream_t * stream, byte_t bits, uint32_t count)
{
	if (stream->capacity == 0) {
		int init_size = 800;
		bit_stream_resize(stream, init_size);
	}

	if (stream->capacity - stream->pos < count) {
		bit_stream_resize(stream, stream->capacity * 2);
	}

	bits <<= 8-count;
	uint32_t bit_left = 8 - stream->pos % 8;
	if (bit_left > 0) {
		stream->data[stream->pos/8] |= bits >> (8-bit_left);
		bits <<= bit_left;
		bit_left = min(bit_left, count);
		count -= bit_left;
		stream->pos += bit_left;
	}

	if (count> 0) {
		stream->data[stream->pos/8] = bits;
		stream->pos += count;
	}
}

void bit_stream_dump(bit_stream_t * stream)
{
	printf("[%d]", stream->pos);
	for (int i = 0 ; i < stream->pos; ++i) {
		byte_t bit = (stream->data[i/8] & (0x80>>(i%8)))!=0;
		putchar('0'+ bit);
	}
	printf("\n");
}
