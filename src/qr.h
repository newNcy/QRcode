#pragma once

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
    unsigned char * mudules;
};
typedef struct qr_t qr_t;

/*
 * 编码器
 * 因为需要存一些中间数据
 */
struct qr_encoder_t
{
    qr_code_mode_enum mode;
    qr_error_correction_level_enum  ec_level;
    int version;
};
typedef struct qr_encoder_t qr_encoder_t;


/*
 * 解码器
 */
struct qr_decoder_t
{
};
typedef struct qr_decoder_t qr_decoder_t;


qr_t * qr_create(qr_encoder_t * encoder, unsigned char * bytes, qr_code_mode_enum mode, qr_error_correction_level_enum ec_level);

void qr_destroy(qr_t * qr);





