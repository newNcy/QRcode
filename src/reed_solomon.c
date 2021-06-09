#include "reed_solomon.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

gf256_t * get_gf256()
{
    static gf256_t gf256;
    static int inited = 0;
    if (!inited) {
        gf256.exp[0] = 1;
        for (int i = 1; i < 256; ++i) {
            int t = gf256.exp[i-1]*2;
            if (t > 255) {
                t ^= GF_PRIM;
            }
            gf256.exp[i] = t;
            gf256.log[gf256.exp[i]] = i;
        }
        gf256.log[1] = 0;
    }

    return &gf256;
}

polynomial_t polynomial_make(uint32_t len)
{
    polynomial_t poly;
    poly.len = len;
    poly.coeffs = (unsigned char*)malloc(len);
	memset(poly.coeffs, 0, len);
    return poly;
}

/*
 * 从字节数组初始化多项式
 * @bytes 系数数组
 * @data_len 系数数组长度
 * @poly_len 多项式长度当两者不一的时候，有效数据为两者最小值，从最高次幂填充
 */
polynomial_t polynomial_from_bytes(unsigned char * bytes, uint32_t data_len, uint32_t poly_len)
{
	polynomial_t ret = polynomial_make(poly_len);
	uint32_t copy_len = min(data_len, poly_len);
	for (int i = 0; i < poly_len; ++i) {
		ret.coeffs[poly_len-1-i] = bytes[i];
	}

	return ret;
}

/*
 * 把所有项的指数增加n,等于乘以X^n
 */
polynomial_t polynomial_extend(polynomial_t a, uint32_t n)
{
    polynomial_t b = polynomial_make(a.len + n);
    for (int i = a.len-1; i >= 0; -- i) {
        b.coeffs[i+n] = a.coeffs[i];
    }

    return b;
}

void polynomial_release(polynomial_t poly)
{
    if (poly.coeffs) {
        free(poly.coeffs);
    }
}

void polynomial_print(polynomial_t poly)
{
	gf256_t * gf = get_gf256();
	printf("[%d] %d(a^%d)", poly.len, poly.coeffs[poly.len-1], gf->log[poly.coeffs[poly.len-1]]);
	for (int i = poly.len - 2; i >=0; --i) {
		printf(" %d(a^%d)", poly.coeffs[i], gf->log[poly.coeffs[i]]);
	}
	printf("\n");
}

/*
 * 计算生成子
 * @len 长度
 * 计算过程为(a^0*x - a^0) * ... * (a^0*x - a^(len-1))
 * 因为在伽罗华域上计算，乘法可以表示成指数相加 加法表示成异或运算
 * a^2 * a^4 = a^(2+4) = a^6 a^6的值在gf256的就是exp[6] 
 * a^2 + a^4 = exp[2] xor exp[4]
 * 其中a = 2
 */
polynomial_t make_generator_polynomial(uint32_t len)
{
    polynomial_t generator = polynomial_make(len+1);
	generator.coeffs[0] = 1;
	generator.coeffs[1] = 1;

	gf256_t * gf = get_gf256();
	
	// 乘以多项式 a^0*x - a^i
	for (int i = 1; i < len; ++i) {
		generator.coeffs[i+1] = 1;
		for (int j = i; j > 0; --j) {
			generator.coeffs[j] = generator.coeffs[j-1]^gf->exp[(i + gf->log[generator.coeffs[j]])%255];
		}
		generator.coeffs[0] = gf->exp[(gf->log[generator.coeffs[0]] + i)%255];
	}
	return generator;
}

