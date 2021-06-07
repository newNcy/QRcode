#include "qr.h"
#include <stddef.h>


/*
 * 编码流程
 * 1. 数据分析 根据编码模式确定合适的版本，因为每个版本能存的数据量不一样
 * 2. 数据编码 确定了版本和纠错等级，把对应信息编码在数据前面 然后数据也进行对应模式的编码
 * 3. 为编码后的数据生成纠错数据
 * 4. 把编码后的数据和纠错码组合成最终的数据流
 */

qr_t * qr_create(qr_encoder_t * encoder, unsigned char * bytes, qr_code_mode_enum mode, qr_error_correction_level_enum ec_level)
{

    return NULL;
}
