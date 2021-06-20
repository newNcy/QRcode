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

    int mask;
};
typedef struct qr_encoder_t qr_encoder_t;


void qr_set_module(qr_t qr, int x, int y, byte_t module)
{
	if (x <0 || y < 0 || qr.size - x < 1 || qr.size - y < 1) {
		assert(x>=0);
		assert(x<qr.size);
		printf("%d %d\n",x, y);
		fflush(stdout);
		char * a = NULL;
		*a = 'b';
		assert(y>=0);
		assert(y<qr.size);
		return;
	}
	qr.modules[y*qr.size + x] = module;
}

byte_t qr_get_module(qr_t qr, int x, int y)
{
	if (x <0 || y < 0 || qr.size - x < 1 || qr.size - y < 1) {
		return QR_MODULE_INVALID;
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
	int pos = 6;
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
		for (int i = 0; i < 6; ++i) {
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
	char * module_char[] = 
    {
        "\e[47m  \e[0m", 
        "  ",
        "\e[45m  \e[0m", 
        "\e[44m  \e[0m", 
        "\e[46m  \e[0m", 
    };
	int border = 2;
	for (int y = -border; y < qr.size+border; ++ y) {
		for (int x = -border; x < qr.size+border; ++ x) {
			printf("%s",module_char[qr_get_module(qr, x, y)]);
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
			int x = qr_alignment_location[version][i];
			int y = qr_alignment_location[version][j];
			if (x>0 && y>0) {
				qr_add_alignment_pattern(qr, x, y);
			}
		}
	}

	//timing patterns
	qr_add_timing_patterns(qr);

	//dark
	qr_set_module(qr, 8,4*(version+1)+9, QR_MODULE_BLACK);

	//reserve for format and version
	qr_add_reserve(qr);
	return qr;
}


/*
 * 生成纠错码
 * 使用长除法 过程需要两个多项式，一个是需要编码的数据，一个是生成多项式
 */
void qr_encoder_generate_error_correction_codeword(qr_encoder_t * encoder)
{
	qr_error_correction_parameter_t ec_param = encoder->ec_param;
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
    //group2
	data_codeword_count = encoder->ec_param.data_codeword_per_block2;
    for (int block = 0; block < encoder->ec_param.group2_blocks; ++ block) {
		byte_t * data_start = encoder->data_codewords.data + data_codeword_count*block + ec_param.group1_blocks * ec_param.data_codeword_per_block1;
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

	if (encoder->interleaved_count <  encoder->data_codeword_count) {
		codeword = encoder->data_codewords.data[encoder->interleave_codeword_id];
		int block_size1 = ec_param.data_codeword_per_block1;
		int block_size2 = ec_param.data_codeword_per_block2;
		int group_size1 = ec_param.data_codeword_per_block1 * ec_param.group1_blocks;
		int group_size2 = ec_param.data_codeword_per_block2 * ec_param.group2_blocks;
		int step = block_size1;
		if (encoder->interleave_codeword_id >= group_size1 && group_size2 > 0) {
			step = block_size2;
		}
		//printf("%d %d %d %d %d\n", block_size1, block_size2, group_size1, group_size2, step);
		encoder->interleave_codeword_id += step;
		int beyond = encoder->interleave_codeword_id - encoder->data_codeword_count;
		if (beyond >= 0) {
			encoder->interleave_codeword_id = beyond + 1;
			if (encoder->interleave_codeword_id >= block_size1) {
				encoder->interleave_codeword_id += group_size1;
			}
		} 		
	}else {
		if (encoder->interleaved_count == encoder->data_codeword_count) {
			encoder->interleave_codeword_id = 0;
		}
		codeword = encoder->ec_codewords.data[encoder->interleave_codeword_id];
		encoder->interleave_codeword_id += ec_param.ec_codeword_per_block;
		int beyond = encoder->interleave_codeword_id - encoder->ec_codeword_count;
		if (beyond >= 0) {
			encoder->interleave_codeword_id = beyond + 1;
		}
	}
	encoder->interleaved_count ++;
	return codeword;
}


byte_t qr_encoder_mask_module(qr_encoder_t * encoder, int x,int y, byte_t origin)
{
    byte_t mask = 0;
    switch(encoder->mask) {
        case 0: mask = (y+x)%2; break;
        case 1: mask = y%2; break;
        case 2: mask = x%3; break;
        case 3: mask = (x+y)%3; break;
        case 4: mask = (int)(floor((double)y/2) + floor((double)x/3))%2; break;
        case 5: mask = (x*y)%2 + (x*y)%3; break;
        case 6: mask = ((x*y)%2 + (x*y)%3)%2; break;
        case 7: mask = ((x+y)%2 + (x*y)%3)%2; break;
    }
    return origin ^ !mask;
}


void qr_encoder_fill_data_modules(qr_encoder_t * encoder, qr_t qr)
{
	qr_encoder_reset_interleave(encoder);
    int remain = 0;
	int x = qr.size-1;
	int y = qr.size-1;
	int upping = 1; 
	int left = 0; //0 up 1 down
	printf("data ");
	for (int i = 0; i < encoder->data_codeword_count; ++i) {
		printf("%x ",encoder->data_codewords.data[i]);
	}
	printf("\necc ");
	for (int i = 0; i < encoder->ec_codeword_count; ++i) {
		printf("%x ",encoder->ec_codewords.data[i]);
	}
	printf("\n");
	while (qr_encoder_interleave_left(encoder)) {
		byte_t codeword = qr_encoder_get_interleaved_codeword(encoder);
		printf("%d ",codeword);
		//bitwise
		for (int i = 0; i < 8; ++ i) {
			byte_t bit = (codeword>>(8-i-1)) & 1;
			if (qr_get_module(qr, x, y) != QR_MODULE_NONE) {
				i --;
			} else {
				if (codeword != QR_MODULE_INVALID) {
					bit = qr_encoder_mask_module(encoder, x, y, bit);
					qr_set_module(qr, x, y, bit);
				} else {
					qr_set_module(qr, x, y, QR_MODULE_INVALID);
				}
			}
			if (upping) {
				if (left) {
					x ++;
					y --;
				}else {
					x --;
				}
				if (y < 0) {
					upping = 0;
                    x -= 2;
				}
			} else {
                if (left) {
                    x ++;
                    y ++;
                } else {
                    x --;
                }
                if (y >= qr.size) {
                    upping = 1;
                    x -= 2;
                }
            }
            if (x == 6) { //vertical timing
                if (left) {
                    x ++;
                } else {
                    x --;
                }
            }
			left ^= 1;
		}
		//qr_print(qr);
        //printf("\n");
	}

}

void qr_encoder_fill_format_version_modules(qr_encoder_t * encoder, qr_t qr)
{
    const char * format = qr_format_and_version[encoder->ec_level*8 + encoder->mask];

    int add = 0;
    for (int i = 0; i < 7; ++i) {
        add = i > 5;
        qr_set_module(qr, i+add, 8, format[i] == '1');
        qr_set_module(qr, 8, i+add, format[14-i] == '1');
    }
    qr_set_module(qr, 8, 8, format[7] == '1');
    qr_set_module(qr, qr.size-8, 8, format[7] == '1');

    for (int i = 0; i < 7; ++i) {
        qr_set_module(qr, 8, qr.size-1-i, format[i] == '1');
        qr_set_module(qr, qr.size-1-i,8,format[14-i] == '1');
    }

    printf("format string %s\n", format);
    //version info
    if (qr.version > 5) {
        const char * version = qr_format_and_version[32 + qr.version -6];
        printf("version string %s\n", version);
        for (int i = 0; i < 6; ++ i) {
            for (int j = 0; j < 3; ++ j) {
                int idx = 17 - (i*3+j);
                qr_set_module(qr, i, qr.size-11+j, version[idx] == '1');
                qr_set_module(qr, qr.size-11+j, i, version[idx] == '1');
            }
        }
    }

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
    encoder.mask = 0;
	encoder.mode = mode;
	encoder.ec_level = ec_level;
	encoder.version = version;
    printf("version %d\n", encoder.version + 1);

	qr_error_correction_parameter_t ec_param = qr_error_correction_parameter[version][ec_level];
	encoder.ec_param = ec_param;

	uint32_t data_codeword_count = ec_param.group1_blocks *ec_param.data_codeword_per_block1 + ec_param.group2_blocks * ec_param.data_codeword_per_block2;
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
        bit_stream_push_bits(&encoder.data_codewords, 0, min(4, require_bits - encoder.data_codewords.pos));
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
	qr_encoder_fill_format_version_modules(&encoder, qr);

	qr_encoder_fill_data_modules(&encoder, qr);

    qr_print(qr);
    return qr;
}

void qr_destroy(qr_t qr)
{
	free(qr.modules);
}
