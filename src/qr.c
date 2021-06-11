#include "qr.h"
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include "reed_solomon.h"

/*
 * 编码器
 * 因为需要存一些中间数据
 */
struct qr_encoder_t
{
    qr_code_mode_enum mode;
    qr_error_correction_level_enum  ec_level;
    int version;

	bit_stream_t data_codewords;
	bit_stream_t ec_codewords;
	qr_error_correction_parameter_t ec_param;
	int data_codeword_count;
	int ec_codeword_count;

	int interleave_group_id;
	int interleave_block_id;
	int interleave_codeword_id;
	int interleaved_count;
};
typedef struct qr_encoder_t qr_encoder_t;


/*
 * 解码器
 */
struct qr_decoder_t
{
};
typedef struct qr_decoder_t qr_decoder_t;



/*
 * 生成纠错码
 * 使用长除法 过程需要两个多项式，一个是需要编码的数据，一个是生成多项式
 */
void qr_encoder_generate_error_correction_codeword(qr_encoder_t * encoder)
{
	uint32_t ec_per_block = encoder->ec_param.ec_codeword_per_block;
	uint32_t ec_codeword_count =  ec_per_block * (encoder->ec_param.group1_blocks + encoder->ec_param.group2_blocks);
	encoder->ec_codeword_count = ec_codeword_count;
	bit_stream_resize(&encoder->ec_codewords, ec_codeword_count * 8);

	polynomial_t generator = make_generator_polynomial(ec_per_block);
	polynomial_print(generator);
	//group1
	int data_codeword_count = encoder->ec_param.data_codeword_per_block1;
	int ec_block_idx = 0;
	for (int block = 0; block < encoder->ec_param.group1_blocks; ++ block) {
		byte_t * data_start = encoder->data_codewords.data + data_codeword_count*block;
		polynomial_t error_code = reed_solomon(generator, data_start, data_codeword_count);
		polynomial_copy_to_bytes(error_code, encoder->ec_codewords.data + ec_block_idx*ec_per_block, ec_per_block);
		encoder->ec_codewords.pos += ec_per_block*8;
		ec_block_idx ++;
	}
}

void qr_encoder_reset_interleave(qr_encoder_t * encoder)
{
	encoder->interleave_group_id = 0;
	encoder->interleave_block_id = 0;
	encoder->interleave_codeword_id = 0;
	encoder->interleaved_count = 0;
}

uint32_t qr_encoder_interleave_left(qr_encoder_t * encoder)
{
	qr_error_correction_parameter_t ec_param = encoder->ec_param;
	return ec_param.data_codeword_per_block1 * ec_param.group1_blocks + ec_param.data_codeword_per_block2 * ec_param.group2_blocks + ec_param.ec_codeword_per_block*(ec_param.group1_blocks+ec_param.group2_blocks) - encoder->interleaved_count;
}

byte_t qr_encoder_get_interleaved_codeword(qr_encoder_t * encoder)
{
	qr_error_correction_parameter_t ec_param = encoder->ec_param;
	assert(encoder->interleaved_count < encoder->data_codeword_count + encoder->ec_codeword_count);
	byte_t codeword;

	if (encoder->interleaved_count >  encoder->data_codeword_count) {
		codeword = encoder->data_codewords.data[encoder->interleave_codeword_id];
		encoder->interleave_codeword_id += group_size;
	}else {
	}
	return codeword;
}

void qr_encoder_fill_modules(qr_encoder_t * encoder, qr_t qr)
{
	int x = qr.size-1;
	int y = qr.size-1;
	while (qr_encoder_interleave_left(encoder)) {
	}
}

void qr_set_module(qr_t qr, int x, int y, byte_t module)
{
	if (x <0 || y < 0 || qr.size - x < 1 || qr.size - y < 1) {
		return;
	}
	qr.modules[y*qr.size + x] = module;
}

byte_t qr_get_module(qr_t qr, int x, int y)
{
	if (x <0 || y < 0 || qr.size - x < 1 || qr.size - y < 1) {
		return 0;
	}
	return qr.modules[y*qr.size + x];
}


void qr_add_finder_pattern(qr_t qr, int x, int y)
{
	if (x < 3 || y < 3 || qr.size - x < 4 || qr.size - y < 4) {
		return;
	}

	for (int i = x-3; i <= x+3; ++ i) {
		for (int j = y-3; j <= y+3; ++ j) {
			if ((i == x-2 || i == x+2) && j > y-3 && j < y+3 ||
				(j == y-2 || j == y+2) && (i > x-3 && i < x+3)) {
				qr_set_module(qr, i, j, QR_MODULE_WHITE);
			}else {
				qr_set_module(qr, i, j, QR_MODULE_BLACK);
			}
		}
	}
}

void qr_add_separators(qr_t qr)
{
	for (int i = 0; i < 8; ++i) {
		qr_set_module(qr, i, 7, QR_MODULE_WHITE);
		qr_set_module(qr, qr.size-i-1, 7, QR_MODULE_WHITE);
		qr_set_module(qr, i, qr.size-8, QR_MODULE_WHITE);

		qr_set_module(qr, 7, i, QR_MODULE_WHITE);
		qr_set_module(qr, 7, qr.size-i-1, QR_MODULE_WHITE);
		qr_set_module(qr, qr.size-8, i, QR_MODULE_WHITE);
	}
}

void qr_add_alignment_pattern(qr_t qr, int x, int y)
{
	if (qr_get_module(qr, x, y) != QR_MODULE_NONE) {
		return;
	}
	for (int i = x-2; i <= x+2; ++ i) {
		for (int j = y-2; j <= y+2; ++ j) {
			if (abs(i-x) <2 && abs(j-y) <2 && (i != x || j != y)) {
				qr_set_module(qr, i, j, QR_MODULE_WHITE);
			} else {
				qr_set_module(qr, i, j, QR_MODULE_BLACK);
			}
		}
	}
}

void qr_add_timing_patterns(qr_t qr)
{
	int pos = 5;
	for (int i = 0; i < qr.size; ++ i) {
		if (qr_get_module(qr, i, pos) == QR_MODULE_NONE) {
			qr_set_module(qr, i, pos, (i+1)%2);
		}
		if (qr_get_module(qr, pos, i) == QR_MODULE_NONE) {
			qr_set_module(qr, pos, i, (i+1)%2);
		}
	}
}

void qr_set_reserve_module(qr_t qr, int x, int y)
{
	if (qr_get_module(qr, x, y) == QR_MODULE_NONE) {
		qr_set_module(qr, x, y, QR_MODULE_RESERVE);
	}
}

void qr_add_reserve(qr_t qr)
{
	//format
	for (int i = 0; i < 8; ++i) {
		qr_set_reserve_module(qr, i, 8);
		qr_set_reserve_module(qr, qr.size-i-1, 8);

		qr_set_reserve_module(qr, 8, i);
		qr_set_reserve_module(qr, 8, qr.size-i-1);
	}
	qr_set_reserve_module(qr, 8, 8);

	//version
	if (qr.version > 5) {
		for (int i = 0; i < 5; ++i) {
			qr_set_reserve_module(qr, i, qr.size-9);
			qr_set_reserve_module(qr, i, qr.size-10);
			qr_set_reserve_module(qr, i, qr.size-11);
			
			qr_set_reserve_module(qr, qr.size-9, i);
			qr_set_reserve_module(qr, qr.size-10, i);
			qr_set_reserve_module(qr, qr.size-11, i);
		}
	}
}

void qr_print(qr_t qr)
{
	//white black none reserve
	char module_char[] = {' ', 'x', '.', '@'};
	for (int x = 0; x < qr.size; ++ x) {
		for (int y = 0; y < qr.size; ++ y) {
			printf(" %c",module_char[qr_get_module(qr, x, y)]);
		}
		printf("\n");
	}
}

qr_t qr_construct_shell(int version)
{
	qr_t qr = {0};
	qr.version = version;
	qr.size = 21+4*version;
	qr.modules = (byte_t*)malloc(qr.size * qr.size);
	memset(qr.modules, QR_MODULE_NONE, qr.size * qr.size);

	//finder pattern
	qr_add_finder_pattern(qr,3,3);
	qr_add_finder_pattern(qr,3,qr.size - 4);
	qr_add_finder_pattern(qr,qr.size-4,3);

	//separators
	qr_add_separators(qr);

	//alignment patterns
	for (int i = 0; i < 7; ++i) {
		for (int j = 0; j < 7; ++j) {
			int x = qr_alignment_location[version][i] - 1;
			int y = qr_alignment_location[version][j] - 1;
			if (x>0 && y>0) {
				qr_add_alignment_pattern(qr, x, y);
			}
		}
	}

	//timing patterns
	qr_add_timing_patterns(qr);

	//dark
	qr_set_module(qr, 4*(version+1)+9,8, QR_MODULE_BLACK);

	//reserve for format and version
	qr_add_reserve(qr);
	qr_print(qr);
	return qr;
}


/*
 * 编码流程
 * 1. 数据分析 根据编码模式确定合适的版本，因为每个版本能存的数据量不一样
 * 2. 数据编码 确定了版本和纠错等级，把对应信息编码在数据前面 然后数据也进行对应模式的编码
 * 3. 为编码后的数据生成纠错数据
 * 4. 把编码后的数据和纠错码组合成最终的数据流
 */

qr_t qr_create(byte_t* bytes, qr_code_mode_enum mode, qr_error_correction_level_enum ec_level)
{

	int version = 0;
	unsigned short len = strlen(bytes);
	for (int v = 0; v < 30; ++ v) {
		if (qr_capacities[v][ec_level][mode] >= len) {
			version = v;
			break;
		}
	}

	qr_encoder_t encoder = {0};
	encoder.mode = mode;
	encoder.ec_level = ec_level;
	encoder.version = version;
    printf("version %d\n", encoder.version + 1);

	qr_error_correction_parameter_t ec_param = qr_error_correction_parameter[version][ec_level];
	encoder.ec_param = ec_param;

	uint32_t data_codeword_count = ec_param.group1_blocks *ec_param.data_codeword_per_block2 + ec_param.group2_blocks * ec_param.data_codeword_per_block2;
	encoder.data_codeword_count = data_codeword_count;

	bit_stream_resize(&encoder.data_codewords, data_codeword_count * 8);


	//mode indicator
	bit_stream_push_bits(&encoder.data_codewords, 1<<mode, 4);
	//char count (byte)
	if (version <= 9) {
		bit_stream_push_bits(&encoder.data_codewords, (byte_t)len, 8);
	} else {
		bit_stream_push_bits(&encoder.data_codewords, len>>8, 8);
		bit_stream_push_bits(&encoder.data_codewords, len, 8);
	}

    //encode bytes
	for (int i = 0; i < len; ++i) {
		bit_stream_push_bits(&encoder.data_codewords, bytes[i], 8);
	}

    uint32_t require_bits = data_codeword_count * 8;
    //如果此时bit string的长度不到require_bits,最多填四个0在末尾
    if (encoder.data_codewords.pos < require_bits) {
        bit_stream_push_bits(&encoder.data_codewords, 0, max(4, require_bits - encoder.data_codewords.pos));
    }
    //如果还不足再加一些0让它是8的倍数，也就是整字节
    if (encoder.data_codewords.pos%8 != 0) {
        bit_stream_push_bits(&encoder.data_codewords, 0, 8 - encoder.data_codewords.pos%8);
    }
    //如果还不够就填充 0xec 0x11 直到填满
    byte_t padding_byte [] = { 0xec, 0x11 };
    int padding_idx = 0;
    while (encoder.data_codewords.pos < require_bits) {
        bit_stream_push_bits(&encoder.data_codewords, padding_byte[padding_idx], 8);
        padding_idx ^= 1;
    }

    //生成纠错码
    qr_encoder_generate_error_correction_codeword(&encoder);

	qr_t qr = qr_construct_shell(version);

	qr_encoder_fill_modules(&encoder, qr);

    return qr;
}
