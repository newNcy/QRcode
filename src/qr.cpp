#include "qr.h"

int qr_XY(QR qr, int x, int y)
{
    x = x>=0? x:0;
    x = x<qr->size? x:qr->size-1;
    y = y>=0? y:0;
    y = y<qr->size? y:qr->size-1;
    return y*qr->size+x;
}

/* 
 * 显示二维码
 */
void qr_view(QR qr)
{

    int w = 1;
    printf("\n[ver-%03d]\n",qr->ver);
    printf("[ecl-'%c']\n","LMQH"[qr->ec]);
    printf("[%03dx%03d]\n",qr->size,qr->size);
    for (int i = 0;i<qr->size+2; i++)
        printf("\033[47m  \033[0m");
        printf("\n");
    for (int i = 0;i<qr->size; i++) {
        printf("\033[47m  \033[0m");
        for (int j = 0; j< qr->size; j++) {
            unsigned module = qr->matrix[qr_XY(qr,j,i)];
            if (module == WHITE_MODULE) {
                    printf("\033[47m  \033[0m");
            }else if(module == BLACK_MODULE){
                    printf("  ");
            }else if (module == RESERVE_MODULE) {
                printf("\033[43m  \033[0m");
            }else if (module == NONE_MODULE) {
                printf("\033[40m  \033[0m");
            }
        }
        printf("\033[47m  \033[0m");
        printf("\n");
    }
    for (int i = 0;i<qr->size+2; i++)
        printf("\033[47m  \033[0m");
    printf("\n");
}


/*
 * 二进制查看字节数组
 */
void bin(char *name, unsigned char b[],int l)
{
    printf("%s(%d):\n",name,l);
    for(int i = 0; i < l;i ++) {
        for (int j = 0;j < 8; j++) {
            printf("%d",(b[i]&(1<<(7-j)))>>(7-j));
        }
        printf(" ");
        if((i+1)%5==0) printf("\n");
    }
    printf("\n");
}

/*
 * 取位运算
 */

#define bit(W,N) (((W)>>(sizeof(W)*8-1-(N)))&1)
/*
char bit(unsigned char word, int ndx) 
{
    return (word>>((sizeof(word)*8-1)-ndx))&1;
}
*/
/*
 * 转大小端
 */
void turn_endian(char b[],int l)
{
    //
    //return;
    for (int i=0; i<l; i++) {
        char t = b[i];
        b[i] = b[l-i-1];
        b[l-i-1] = t;
    }
}
/*
 * 编码数据
 */
void qr_encode_bytes(QR qr, const char bytes[])
{
    short len = strlen(bytes);
    //bin("bytes",(unsigned char *)bytes,len);
    //bin("len",(unsigned char*)&len,2);
    /* 
     * 码字长度 = 版本容量 + 4bit模式指示位 + 8bit信息长度(版本10以及以上为16bit)
     */
    int * ecp = ec_parameter[QR_VERSION(qr->ver)][qr->ec];
    //printf("max:%d\n",qr->data_max);
    memset(qr->data,0,qr->data_max);
    int index = 0;
    qr->data[index] = 64; // 0100 0000 *0100是utf-8字节编码模式
    
    /*
     * v10以及以上的长度用16个bit存储
     */
    if (qr->ver >= 10) {
        turn_endian((char*)&len,2);
        qr->data[index] = (qr->data[index] & 0xf0) | ((len>>12)&0x0f);
        index++;
        qr->data[index++] = ((len&(0x0f<<8))>>4) | ((len&0xf0)>>4);
        qr->data[index] |= (len&0x0f)<<4;
    }else {
        qr->data[index++] |= ((char)len&0xf0)>>4;
        qr->data[index] |= ((char)len&0x0f)<<4;
    }


    /*
     * 把每个字节分成两半, 一次放4bit到bit流
     */
    for(int i = 0; i < len; i ++) {
        qr->data[index++] |= (bytes[i]&0xf0)>>4;
        qr->data[index] |= (bytes[i]&0x0f)<<4;
    }
    /* 因为已经初始化为0000.....直接跳过就补齐8的倍数了 */
    index++;

    /*
     * 填充pad byte 直到长度为数据码字总数
     */
    unsigned char pad[] = {0xec, 0x11}; // 1110 1100,0001 0001
    int pad_count = qr->data_max - index;
    for (int i = 0; i < pad_count; i ++) {
        qr->data[index++] = pad[i%2];
    }

    //bin("encoded",qr->data, index);
 
}


/*
 * 为各个block的数据码生成纠错码
*/
void qr_error_correct_interleave(QR qr)
{
   
    int * ecp = ec_parameter[QR_VERSION(qr->ver)][qr->ec];
    //printf("%d:%d:%d:%d:%d\n",ecp[0],ecp[1],ecp[2],ecp[3],ecp[4]);

    GF gf;
    gf.len = GF_MAX;
    gf_make(&gf);

    /* 
     * 所罗门纠错码测试区
     *
    unsigned char msg [] = {32, 91, 11, 120, 209, 114, 220, 77, 67, 64, 236, 17, 236, 17, 236, 17};

    unsigned char g[10] = {0};
    gf_make_generator_poly(&gf,g,10);
    poly_view("gen",g,10);
    unsigned char err[10] = {0};
    reed_solomon(&gf,msg,0,16,err,10,g);
    poly_view("err",err,10);
     * 
     * 测试区
     */
    
    int ec_per_block = ecp[0];
    int ec_index = 0;
    int interleave_ndx = 0;
    int block_id = 0;
    unsigned char * ec_codeword = (unsigned char*)malloc(ec_per_block);
    unsigned char * generator = (unsigned char*)malloc(ec_per_block);
    memset(generator,0,ec_per_block);
    gf_make_generator_poly(&gf,generator,ec_per_block);

    for (int group_ndx = 0; group_ndx<2; group_ndx++) {
        int block_per_group = ecp[1 + group_ndx*2];
        int data_per_block = ecp[2 + group_ndx*2];
        for (int block_ndx = 0; block_ndx < block_per_group; block_ndx++) {
            reed_solomon(&gf,(qr->data+ec_index),data_per_block,ec_codeword,ec_per_block,generator);

            /* 数据交错 */
            interleave_ndx = block_id;
            //block 的每一个数
            for (int i = 0; i < data_per_block; i++) {
                qr->codeword[interleave_ndx] = qr->data[ec_index + i];

                //跳到下一个block的待取位置
                if (i+1 < ecp[2]) {
                    interleave_ndx += ecp[1]; //第一组block数
                }
                if (i+1 < ecp[4]) {
                    interleave_ndx += ecp[3]; //第二组block数
                }

            }
            //poly_view("raw",qr->data+ec_index,data_per_block);
            //poly_view("ec",ec_codeword,ec_per_block);


            interleave_ndx = block_id + ecp[1]*ecp[2]+ecp[3]*ecp[4];
            for (int i = 0; i < ec_per_block; i++) {
                qr->codeword[interleave_ndx] = ec_codeword[i];
                interleave_ndx+=(ecp[1]+ecp[3]);
            }
            block_id ++;
            ec_index+=data_per_block;
        }
    }

    free(generator);
    free(ec_codeword);
}

/*
 * 添加识别块
 */
void qr_add_finder_pattern(QR qr)
{
    pos ps[3];
    ps[0] = (pos){0,0};
    ps[1] = (pos){qr->size-7,0};
    ps[2] = (pos){0,qr->size-7};
    for (int i = 0; i < 3; i ++) {
        int cx = ps[i].x + 3;
        int cy = ps[i].y + 3;
        qr->matrix[qr_XY(qr,cx,cy)] = BLACK_MODULE;
        for(int j = 1; j<=3 ;j++) {
            int sx,sy;
            for ( sx = cx-j,sy=cy-j; sx<=cx+j; sx++,sy++) {
                qr->matrix[qr_XY(qr,sx,cy-j)] = j%2?BLACK_MODULE:WHITE_MODULE;
                qr->matrix[qr_XY(qr,sx,cy+j)] = j%2?BLACK_MODULE:WHITE_MODULE;
                qr->matrix[qr_XY(qr,cx+j,sy)] = j%2?BLACK_MODULE:WHITE_MODULE;
                qr->matrix[qr_XY(qr,cx-j,sy)] = j%2?BLACK_MODULE:WHITE_MODULE;
            }
        }
    }
}
/*
 * 添加分割线
 */
void qr_add_separator(QR qr)
{
    for (int i = 0; i< 8; i++) {
        qr->matrix[qr_XY(qr,7,i)] = WHITE_MODULE;
        qr->matrix[qr_XY(qr,i,7)] = WHITE_MODULE;

        qr->matrix[qr_XY(qr,i-8+qr->size,7)] = WHITE_MODULE;
        qr->matrix[qr_XY(qr,qr->size-8,i)] = WHITE_MODULE;

        qr->matrix[qr_XY(qr,i,qr->size-8)] = WHITE_MODULE;
        qr->matrix[qr_XY(qr,7,qr->size-8+i)] = WHITE_MODULE;
    }
}

/*
 * 添加定位块 
 */

void qr_add_align_pattern(QR qr) 
{
    if (qr->ver < 2) return;
    int * aligns = align_location[QR_VERSION(qr->ver)];
    for (int i = 0; i < 7; i ++) {
        if (aligns[i] != 0)
        for (int j = 0; j < 7; j ++) {
            if (aligns[j] != 0) {
                int cx = aligns[j];
                int cy = aligns[i];
                int c = qr->matrix[qr_XY(qr,cx,cy)];
                if ( c == NONE_MODULE ) {
                    qr->matrix[qr_XY(qr,cx,cy)] = BLACK_MODULE;
                    for(int k = 1; k <= 2; k ++) {
                        int sx,sy;
                        for ( sx = cx-k,sy=cy-k; sx<=cx+k; sx++,sy++) {
                            qr->matrix[qr_XY(qr,sx,cy-k)] = k%2^1?BLACK_MODULE:WHITE_MODULE;
                            qr->matrix[qr_XY(qr,sx,cy+k)] = k%2^1?BLACK_MODULE:WHITE_MODULE;
                            qr->matrix[qr_XY(qr,cx+k,sy)] = k%2^1?BLACK_MODULE:WHITE_MODULE;
                            qr->matrix[qr_XY(qr,cx-k,sy)] = k%2^1?BLACK_MODULE:WHITE_MODULE;
                        }
                    }
                }
            }
        }
    }
}

/*
 * 添加坐标尺
 */
void qr_add_timing_pattern(QR qr)
{
    for ( int i = 8; i < qr->size-8; i++ ) {
        qr->matrix[qr_XY(qr,6,i)] = (i-7)%2?BLACK_MODULE:WHITE_MODULE;
        qr->matrix[qr_XY(qr,i,6)] = (i-7)%2?BLACK_MODULE:WHITE_MODULE;
    }
}


/*
 * 添加黑块跟信息区域 
 */
void qr_add_dark_reserve(QR qr)
{
    qr->matrix[qr_XY(qr,8,4*qr->ver+9)] = BLACK_MODULE;
    /*
     * 格式信息保留区
     */
    for (int i = 0; i < 8; i++) {
        if (qr->matrix[qr_XY(qr,8,i)] == NONE_MODULE) {
            qr->matrix[qr_XY(qr,8,i)] = RESERVE_MODULE;
        }
        if (qr->matrix[qr_XY(qr,i,8)] == NONE_MODULE) {
            qr->matrix[qr_XY(qr,i,8)] = RESERVE_MODULE;
        }
        if (qr->matrix[qr_XY(qr,qr->size-1-i,8)] == NONE_MODULE) {
            qr->matrix[qr_XY(qr,qr->size-1-i,8)] = RESERVE_MODULE;
        }
        if (qr->matrix[qr_XY(qr,8,qr->size-1-i)] == NONE_MODULE) {
            qr->matrix[qr_XY(qr,8,qr->size-1-i)] = RESERVE_MODULE;
        }
    }
     qr->matrix[qr_XY(qr,8,8)] = RESERVE_MODULE;

     if (qr->ver >= 7) {
         for (int i = 0; i < 6; i ++ ) {
             for (int j = 0; j < 3; j++ ) {
                 qr->matrix[qr_XY(qr,qr->size-9-j,i)] = RESERVE_MODULE;
                 qr->matrix[qr_XY(qr,i,qr->size-9-j)] = RESERVE_MODULE;
             }
         }
     }

}

/* 
 * 使用掩模
 */
char mask(QR qr,int x,int y,unsigned char module)
{
    int i = y;
    int j = x;
    switch(qr->mask) {
    case QR_MASK_000:
        return ((i+j)%2 == 0)^module;
    case QR_MASK_001:
        return (i%2 == 0)^module;
    case QR_MASK_010:
        return (j%3 == 0)^module;
    case QR_MASK_011:
        return ((i+j)%3 == 0)^module;
    case QR_MASK_100:
        return ((i/2+j/3)%2 == 0)^module;
    case QR_MASK_101:
        return ((i*j)%2+(i*j)%3 == 0)^module;
    case QR_MASK_110:
        return (((i*j)%2+(i*j)%3)%2 == 0)^module;
    case QR_MASK_111:
        return (((i*j)%3+(i+j)%2)%2 == 0)^module;
    default:
        return module;
    }
}
/*
 * 添加数据到二维码矩阵
 */
void qr_add_data_bit(QR qr)
{
    int x = qr->size - 1;
    int y = qr->size - 1;
    int left = -1;
    int right = 1;
    int up = -1;
    int down = 1;
    int bit_n = 8;
    int state = right;
    int side = up;

    //bin("interleave",qr->codeword,codeword_count[QR_VERSION(qr->ver)]);

    for (int i = 0; i <= qr->codeword_max; i++ ) {
        if (i == qr->codeword_max) bit_n = remainder_bit_count[QR_VERSION(qr->ver)];
        for (int j = 0; j < bit_n; j ++) {
            /* 寻找下一个未使用的格子 */
            while (qr->matrix[qr_XY(qr,x,y)] != NONE_MODULE) {
                if (state == right) {
                    state = left;
                    x-=1;
                }else {
                    state = right;
                    x+=1;
                    y+=side;
                }
                //if (bit_ndx == qr->size*qr->size-1) return;
                if ( (side == up && y == -1) || (side == down && y == qr->size) ) {

                    x -= 2;
                    y-=side;
                    
                    /* !!!重要的事情  碰到垂直坐标线直接跳过这一列而不是寻找下一个可用的格子 */
                    if (x == 6) x--;
                    side *= -1;
                }
            }
            int b = bit(qr->codeword[i], j);
            if (i == qr->codeword_max) b = 0;
            qr->matrix[qr_XY(qr,x,y)] = mask(qr,x,y,b)==1?BLACK_MODULE:WHITE_MODULE;
        }
    }
}


/* 
 * 添加格式信息和版本信息
 */

void qr_add_format_version(QR qr)
{
    unsigned char * format = format_string[qr->ec][qr->mask];
    turn_endian((char*)&format,4);
    printf("format: %s\n",format);

    int format_ndx = 14;
    /* 添加格式信息 */
    
    for (int i = 0; i < 9; i++) {
        unsigned char module = qr->matrix[qr_XY(qr,8, i)];
        if (module == RESERVE_MODULE) {
            qr->matrix[qr_XY(qr,8,i)] = format[format_ndx--]=='1'?BLACK_MODULE:WHITE_MODULE;
        }
    }
    for (int i = 7; i >= 0; i--) {
        unsigned char module = qr->matrix[qr_XY(qr,i,8)];
        if (module == RESERVE_MODULE) {
            qr->matrix[qr_XY(qr,i,8)] = format[format_ndx--]=='1'?BLACK_MODULE:WHITE_MODULE;
        }
        
    }
    

    format_ndx=14;
    for (int i = 0; i < 8; i++ ) {
        qr->matrix[qr_XY(qr,qr->size-1-i,8)] = format[format_ndx--]=='1'?BLACK_MODULE:WHITE_MODULE;
    }

    for (int i = 6; i >= 0; i--) {
        qr->matrix[qr_XY(qr,8,qr->size-1-i)] = format[format_ndx--]=='1'?BLACK_MODULE:WHITE_MODULE;
    }

    /* 
     * 版本信息
     */
    if (qr->ver >= 7) {
        unsigned char *version = QR_VERSION_STRING(qr->ver);
        int version_ndx = 17;
        for (int i = 0; i < 6; i ++) {
            for (int j = 0; j < 3; j++) {
                qr->matrix[qr_XY(qr,i,qr->size-11+j)] = version[version_ndx--]=='1'?BLACK_MODULE:WHITE_MODULE;
                version_ndx++;
                qr->matrix[qr_XY(qr,qr->size-11+j,i)] = version[version_ndx--]=='1'?BLACK_MODULE:WHITE_MODULE;
            }
        }
    }
}
/*
 * 初始化qrcode 
 */
 QR qr_create( const char * bytes,enum QR_EC_LEVEL ec)
{
    int msg_length = strlen(bytes);
    int ver = 0;
    for (int i = 1; i<= QR_VERSION_MAX; i ++) {
        if(capacity[QR_VERSION(i)][QR_ENCODE_TYPE_BYTE][ec] >= msg_length) {
            ver = i;
            break;
        } 
    }
    if (ver == 0) {
        printf("没有合适的版本");
        return NULL;
    }

    QR qr = (QR)malloc(sizeof(struct _qr_));
    qr->ver = ver;
    qr->ec = ec;
    qr->mode = QR_ENCODE_TYPE_BYTE;
    qr->size = QR_SIZE(ver);
    int * ecp=ec_parameter[QR_VERSION(ver)][ec];
    qr->data_max = ecp[1]*ecp[2]+ecp[3]*ecp[4];
    qr->codeword_max = codeword_count[QR_VERSION(ver)];
    qr->mask = QR_MASK_001;

    qr->data = (unsigned char *)malloc(qr->data_max);
    qr->matrix = (unsigned char *)malloc(qr->size*qr->size);
    qr->codeword = (unsigned char *)malloc(qr->codeword_max);
    memset(qr->codeword,0,codeword_count[QR_VERSION(ver)]);
    for (int i=0;i<qr->size; i++) {
        for (int j=0;j<qr->size; j++) {
            qr->matrix[qr_XY(qr,i,j)] = NONE_MODULE;
        }
    }

    qr_encode_bytes(qr,bytes);
    qr_error_correct_interleave(qr);
    qr_add_finder_pattern(qr);
    qr_add_separator(qr);
    qr_add_align_pattern(qr);
    qr_add_timing_pattern(qr);
    qr_add_dark_reserve(qr);
    qr_add_data_bit(qr);
    qr_add_format_version(qr);
    return qr;
}

//输出为json（方便）
int qr_json_len(QR qr,const char *name)
{
    int width = qr->size;
    /**
     * {[  [0,1,0,...1(size个)],         [],[],...[](size个)  ]}
     */
    int len = 4 +2*(width+1)*width;
    len+= strlen(name)+3;//"":
    return  len;
}

void qr_to_json(QR qr,const char * name,char * json)
{

    int idx = 0;
    

    json[idx++] = '{';
    json[idx++] = '\"';
    strcpy(json+idx,name);
    idx += strlen(name);
    json[idx++] = '\"';
    json[idx++] = ':';
    json[idx++] = '[';


    for (int i = 0; i <  qr->size; i++) {
        json[idx++] = '[';
        for (int j = 0; j < qr->size; j++) {
            json[idx++] = qr->matrix[qr_XY(qr,j,i)]==BLACK_MODULE?'1':'0';
            if (j!=qr->size-1) {
                json[idx++] = ',';
            }
        }
        json[idx++] = ']';
        if (i != qr->size-1) {
            json[idx++] = ',';
        }
    }
    json[idx++] = ']';
    json[idx++] = '}';
    json[idx++] = 0;
    printf("%s\n",json);
}
/*
 * 销毁一个qr 结构体
 */
void qr_destroy(QR qr)
{
    free(qr->data);
    free(qr->codeword);
    free(qr->matrix);
    free(qr);
}
