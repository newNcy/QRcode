#pragma once
#include "standard_table.h"
#include "bit_stream.h"

enum qr_code_mode_enum
{
	NUMERIC,
	ALPHANUMERIC ,
	BYTE,
	KANJI,
	EXTENDED_CHANNEL_INTERPRETATION,
	STRUCTURED_APPEND,
	FNC1,
};
typedef enum qr_code_mode_enum qr_code_mode_enum;

enum qr_error_correction_level_enum
{
    ERROR_CORRECTION_L,
    ERROR_CORRECTION_M,
    ERROR_CORRECTION_Q,
    ERROR_CORRECTION_H,
};

typedef enum qr_error_correction_level_enum qr_error_correction_level_enum;

/*
 * 最后编码得到的QR结构体 
 * 包含版本信息和格子数据
 */
struct qr_t
{
    int version;
    byte_t * mudules;
};
typedef struct qr_t qr_t;

qr_t qr_create(byte_t * bytes, qr_code_mode_enum mode, qr_error_correction_level_enum ec_level);

void qr_destroy(qr_t qr);





