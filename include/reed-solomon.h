#ifndef _REED_SOLOMON_H
#define _REED_SOLOMON_H
#include <stdio.h>
#include <malloc.h>
/* 
 * 伽罗华域 
 */
typedef struct _gf_2_8_
{
    int len;
    unsigned char exp[256];
    unsigned char log[256];
}GF_2_8;
#define GF GF_2_8
#define GF_M 8
#define GF_MAX (2<<(GF_M-1)) /* 2的GF_M次方 */
#define GF_PRIM 285  //本原多项式

/*
 * 生成伽罗华域
 * @ gf 要构造的域
 */
void gf_make(GF *  gf);

void poly_view(char *name,unsigned char *p,int l);

/* 
 * 生成 生成多项式
 * @ gf 所属的域
 * @ gen 存储生成多项式
 * @ len 纠错码个数，即多项式长度-1(首项系数指数为恒0,不存储)
 */
void gf_make_generator_poly(GF * gf,unsigned char gen[],unsigned int len);

/*
 * 生成所罗门纠错码
 * @ gf 所属的域
 * @ msg 要编码的字节串
 * @ moff msg的偏移
 * @ msg 的字节个数
 * @ err 存放纠错码
 * @ elen 纠错码个数
 * @ g g(x) 多项式(生成多项式)
 */
void reed_solomon(GF * gf, unsigned char  msg[], int mlen, unsigned char err[],int elen,unsigned char g[]); 
#endif
