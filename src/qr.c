#include "qr.h"
#include <stddef.h>
#include <string.h>
#include <stdio.h>
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
};
typedef struct qr_encoder_t qr_encoder_t;


/*
 * 解码器
 */
struct qr_decoder_t
{
};
typedef struct qr_decoder_t qr_decoder_t;



void qr_encoder_generate_error_correction_codeword(qr_encoder_t * encoder)
{
	int * ec_params = qr_error_correction_parameter[encoder->version][encoder->ec_level];
	uint32_t ec_codeword_count = ec_params[0] * (ec_params[1] + ec_params[3]);
	bit_stream_resize(&encoder->ec_codewords, ec_codeword_count * 8);
	bit_stream_dump(&encoder->data_codewords);
	bit_stream_dump(&encoder->ec_codewords);

    gf256_t * gf256 = get_gf256();
    for (int i = 0 ; i < 256; ++i) {
        printf("%d %d\n", gf256->exp[i], gf256->log[i]);
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
	qr_t qr = {0};
	qr.version = 0;
	unsigned short len = strlen(bytes);
	for (int v = 0; v < 30; ++ v) {
		if (qr_capacities[v][ec_level][mode] >= len) {
			qr.version = v;
			break;
		}
	}

	qr_encoder_t encoder = {0};
	encoder.mode = mode;
	encoder.ec_level = ec_level;
	encoder.version = qr.version;
    printf("version %d\n", encoder.version + 1);

	int * ec_params = qr_error_correction_parameter[qr.version][ec_level];

	uint32_t data_codeword_count = ec_params[1]*ec_params[2] + ec_params[3]*ec_params[4];

	bit_stream_resize(&encoder.data_codewords, data_codeword_count * 8);


	//mode indicator
	bit_stream_push_bits(&encoder.data_codewords, 1<<mode, 4);
	//char count (byte)
	if (qr.version <= 9) {
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

    return qr;
}
