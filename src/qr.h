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

enum qr_module_enum
{
	QR_MODULE_WHITE,
	QR_MODULE_BLACK,
	QR_MODULE_NONE,
	QR_MODULE_RESERVE,
};

/*
 * 最后编码得到的QR结构体 
 * 包含版本信息和格子数据
 */
struct qr_t
{
    int version;
	int size;
    byte_t * modules;
};
typedef struct qr_t qr_t;

qr_t qr_create(byte_t * bytes, qr_code_mode_enum mode, qr_error_correction_level_enum ec_level);
void qr_print(qr_t qr);

void qr_destroy(qr_t qr);





