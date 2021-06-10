#pragma once
#include <stdint.h>

#include "defs.h"
/* 
 * 多项式
 * @len 项数
 * @coeffs 系数 
 * 举例 3x^2 - 5x^1 + 2
 */
struct polynomial_t 
{
    uint32_t len;
    unsigned char * coeffs;
};

typedef struct polynomial_t polynomial_t;

polynomial_t polynomial_make(uint32_t len);
polynomial_t polynomial_from_bytes(unsigned char * bytes, uint32_t data_len, uint32_t poly_len);
void polynomial_copy_to_bytes(polynomial_t poly, byte_t * bytes, uint32_t buff_len);
void polynomial_print(polynomial_t poly);
void polynomial_release(polynomial_t poly);
polynomial_t polynomial_extend(polynomial_t a, uint32_t n);


#define GF_PRIM 285
struct gf256_t 
{
    unsigned char exp[256];
    unsigned char log[256];
};

typedef struct gf256_t gf256_t;



gf256_t * get_gf256();

polynomial_t make_generator_polynomial(uint32_t len);
polynomial_t reed_solomon(polynomial_t generator, byte_t * data, uint32_t data_len);
