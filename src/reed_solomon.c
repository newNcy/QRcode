#include "reed_solomon.h"

#include <stdlib.h>

polynomial_t polynomial_make(uint32_t len)
{
    polynomial_t poly;
    poly.len = len;
    poly.coeffs = (unsigned char*)malloc(len);
    return poly;
}

void polynomial_release(polynomial_t * poly)
{
    if (poly->coeffs) {
        free(poly->coeffs);
        poly->coeffs = NULL;
        poly->len = 0;
    }
}

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
    }

    return &gf256;
}

/*
 * 计算生成子
 */
polynomial_t make_generator_polynomial(uint32_t len)
{
    polynomial_t generator = polynomial_make(len);
}
