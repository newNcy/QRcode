#pragma once
#include <stdint.h>

/* 
 * 多项式
 * @len 项数
 * @coeffs 系数 
 * 举例 3x^2 - 5x^1 + 2 项数是3 系数是 [2, -5, 3], 反过来是因为这样元素下标就是它的幂
 */
struct polynomial_t 
{
    uint32_t len;
    unsigned char * coeffs;
};

typedef struct polynomial_t polynomial_t;

polynomial_t polynomial_make(uint32_t len);
void polynomial_release(polynomial_t * poly);


#define GF_PRIM 285
struct gf256_t 
{
    unsigned char exp[256];
    unsigned char log[256];
};

typedef struct gf256_t gf256_t;



gf256_t * get_gf256();

polynomial_t make_generator_polynomial(uint32_t len);
