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
 * 域上乘运算 
 */
char gf_mul(GF *gf,char e1, char e2)
{
    return (e1+e2)%gf->len+(e1+e2)/gf->len;
}
/*
 * 域上的加运算
 */
char gf_add(GF *gf,char e1, char e2)
{
    char e =  gf->exp[e1] ^ gf->exp[e2];
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
void poly_view(char *name,char p[], int l)
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
void gf_make_generator_poly(GF * gf, char gen[],unsigned int len)
{
    /* a^0*x^1 + a^0*x^0   此时x^1  x^0的alpha的指数都为0*/
    gen[0] = 0;  

    int plen = 1; // now is len of gen poly
    for (int i = 1; i< len;i++) {
        plen++;
        gen[plen-1] = gf_add(gf,gen[plen-2],i);
        for (int j = plen-2; j > 0; j--) {
            gen[j] = gf_add(gf,gen[j-1],gf_mul(gf,gen[j],i));  
        }
        gen[0] = gf_mul(gf,gen[0],i);
    }
    for (int i= 0; i< len/2; i++) {
        char t = gen[i];
        gen[i] = gen[len-i-1];
        gen[len-i-1] = t;
    }
}

/* 
 * 里德-所罗门编码
 */
void reed_solomon(GF * gf, char  msg[], int moff, int mlen, char err[],int elen,char g[]) 
{
    char * mpoly = (char *)malloc(mlen);
    char lead_term = 0; //最高次项系数  初始为0(a^0x^elen)
    for (int i=0; i<mlen;i++) {
        mpoly[i] = msg[i+moff];
    }

    //poly_view("msg_poly",mpoly,mlen);
    //poly_view("gen_poly",g,elen);
    for (int i = 0; i<mlen;i++) {
        //printf("第%d次计算:\n",i+1);
        char e = gf->log[mpoly[0]];
            //poly_view("e",&e,1);
        int j;
        for (j=1;j<elen+1;j++) {
            char t  = (g[j-1]+e)%255;
            mpoly[j] = mpoly[j]^gf->exp[t];
        }
    
        for (j=0;j<mlen;j++) {
            mpoly[j] = mpoly[j+1];
        }
        //poly_view("msg_poly",mpoly,mlen);
        //poly_view("gen_poly",gp,elen+1);
    }
    for (int i = 0;i<elen;i++) {
        err[i] = mpoly[i];
    }
    free(mpoly);
}

