#include "reed-solomon.h"

/**
 * 构造一个galois域
 */
void gf_make(GF *  gf)
{
    gf->exp[0] = 1;
    gf->exp[gf->len-1] = 1;
    gf->log[0] = 0;
    gf->log[1] = 0;

    for (int i = 1;i < gf->len-1; i++ ) {
        int t = gf->exp[i-1]*2;
        if (t > 255)
            gf->exp[i] = t^GF_PRIM;
        else 
            gf->exp[i] = t;
        gf->log[gf->exp[i]] = i;
    }
}

/* 
 * 域上乘运算 (仅在计算生成多项式使用，计算纠错码直接队255求余)
 */
unsigned char gf_mul(GF *gf,unsigned char e1, unsigned char e2)
{
    return (e1+e2)%gf->len+(e1+e2)/gf->len;
}
/*
 * 域上的加运算
 */
unsigned char gf_add(GF *gf,unsigned char e1, unsigned char e2)
{
    unsigned char e =  gf->exp[e1] ^ gf->exp[e2];
    return gf->log[e];
}

/*
 * 打印域
 */
void gf_view(GF* gf)
{
    printf("exp:\n");
    for (int i= 0; i< gf->len; i++) {
        printf("%3d ",gf->exp[i]);
        if ((i+1)%8 == 0) printf("\n");
    }
    printf("\n");
    printf("log:\n");
    for (int i= 0; i< gf->len; i++) {
        printf("%3d ",gf->log[i]);
        if ((i+1)%8 == 0) printf("\n");
    }
    printf("\n");
}

/* 打印多项式
 */
void poly_view(char *name,unsigned char p[], int l)
{
    printf("%s:\n",name);
    for (int i= 0;i<l;i++) {
        printf("%3d ",p[i]);
        //if(i!=l) printf(" + ");
        if ((i+1)%8==0) printf("\n");
    }
    printf("\n");
}

/*
 * 构造生成多项式 
 */
void gf_make_generator_poly(GF * gf, unsigned char gen[],unsigned int len)
{
    /* a^0*x^1 + a^0*x^0   此时x^1  x^0的alpha的指数都为0*/
    //gen[0] = 0;  

    int plen = 1; // 多项式长度
    for (int i = 1; i< len;i++) {
        plen++;
        gen[plen-1] = gf_add(gf,gen[plen-2],i);
        for (int j = plen-2; j > 0; j--) {
            gen[j] = gf_add(gf,gen[j-1],gf_mul(gf,gen[j],i)); //本身系数加上指数减一的系数(因为乘(a^0x^1+a^ix^0)) 
        }
        gen[0] = gf_mul(gf,gen[0],i);
    }
    /* 倒换顺序  最高次幂系数在最前面 */
    for (int i= 0; i< len/2; i++) {
        char t = gen[i];
        gen[i] = gen[len-i-1];
        gen[len-i-1] = t;
    }
}

/* 
 * 里德-所罗门编码
 */
void reed_solomon(GF * gf, unsigned char*  msg, int mlen,unsigned char err[],int elen,unsigned char g[]) 
{ 
    /* 除法运算空间 */
    int max_len = mlen>elen?mlen:elen;
    unsigned char * mpoly = (unsigned char *)malloc(max_len);
    memset(mpoly,0,max_len);
    unsigned lead_term = 0; //最高次项系数  初始为0(a^0x^elen)
    for (int i=0; i<mlen;i++) {
        mpoly[i] = msg[i];
    }

    for (int i = mlen;i < elen; i++) {
        mpoly[i] = 0;
    }
            
	for (int  i=0; i <  mlen; i++) { // mlen次计算
        if (mpoly[0] == 0) { //首项为0,可能前面被消除了
            for(int j = 0; j < mlen; j++) {
                mpoly[j] = mpoly[j+1];
            }
        }else { //首项不为0则消项 
            int e = gf->log[mpoly[0]]; //首项指数
            /* 
             * 用mpoly的前elen项
             * 异或gen前elen项乘以a的e次方(指数模255)
             */
            mpoly[0] = 0;
            for (int j = 1; j<=elen; j++) {
                int term = gf->exp[(g[j-1]+e)%255];
                mpoly[j] ^= term;
            }

            /* 
             * 消项后移动系数 
             */
            for(int j = 0; j <=max_len; j++) {
                mpoly[j] = mpoly[j+1];
            }
        }
    }
    
    /* 取elen个 */
    for (int i= 0; i< elen; i++) {
        err[i] = mpoly[i];
    }
    
    free(mpoly);
}

