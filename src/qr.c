#include "qr.h"
#include <stddef.h>
#include <string.h>
#include <stdio.h>

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
	qr.version = 1;
	unsigned short len = strlen(bytes);
	for (int v = 0; v < 30; ++ v) {
		if (qr_capacities[v][ec_level][mode] >= len) {
			qr.version = v + 1;
			break;
		}
	}

	qr_encoder_t encoder = {0};
	encoder.mode = mode;
	encoder.ec_level = ec_level;
	encoder.version = qr.version;

	int * ec_params = qr_error_correction_parameter[qr.version][ec_level];

	uint32_t data_codeword_count = ec_params[1]*ec_params[2] + ec_params[3]*ec_params[4];
	uint32_t ec_codeword_count = ec_params[0] * (ec_params[1] + ec_params[3]);

	bit_stream_resize(&encoder.data_codewords, data_codeword_count * 8);
	bit_stream_resize(&encoder.ec_codewords, ec_codeword_count * 8);


	//mode indicator
	bit_stream_push_bits(&encoder.data_codewords, 1<<mode, 4);
	//char count (byte)
	if (qr.version <= 9) {
		bit_stream_push_bits(&encoder.data_codewords, (byte_t)len, 8);
	} else {
		bit_stream_push_bits(&encoder.data_codewords, len>>8, 8);
		bit_stream_push_bits(&encoder.data_codewords, len, 8);
	}

	for (int i = 0; i < len; ++i) {
		bit_stream_push_bits(&encoder.data_codewords, bytes[i], 8);
	}

	bit_stream_dump(&encoder.data_codewords);
    return qr;
}
