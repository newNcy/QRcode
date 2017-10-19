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
    printf("\n[ver-%03d]\n",qr->ver);
    printf("[ecl-'%c']\n","LMQH"[qr->ec]);
    printf("[%03dx%03d]\n",qr->size,qr->size);
    for (int i = 0;i<qr->size+2; i++)
        printf("\033[47m  \033[0m");
        printf("\n");
    for (int i = 0;i<qr->size; i++) {
        printf("\033[47m  \033[0m");
        for (int j = 0; j< qr->size; j++) {
            if (qr->matrix[qr_XY(qr,j,i)] == 0) {
                printf("\033[47m  \033[0m");
            }else if(qr->matrix[qr_XY(qr,j,i)] == 1){
                printf("  ");
            }else if (qr->matrix[qr_XY(qr,j,i)] == 255) {
                printf("\033[40m  \033[0m");
            }
        }
        printf("\033[47m  \033[0m");
        printf("\n");
    }
    for (int i = 0;i<qr->size+2; i++)
        printf("\033[47m  \033[0m");
}



void bin(char *name, char b[],int l)
{
    printf("%s:\n",name);
    for(int i = 0; i < l;i ++) {
        for (int j = 0;j < 8; j++) {
            printf("%d",(b[i]&(1<<(7-j)))>>(7-j));
        }
        printf(" ");
        if((i+1)%5==0) printf("\n");
    }
    printf("\n");
}

void turn_endian(char b[],int l)
{
    for (int i=0; i<l; i++) {
        char t = b[i];
        b[i] = b[l-i-1];
        b[l-i-1] = t;
    }
}
/*
 * 编码数据
 */
void qr_encode_bytes(QR qr, char bytes[])
{
    short len = strlen(bytes);
    bin("bytes",bytes,len);
    bin("len",(char*)&len,2);
    /* 
     * 码字长度 = 版本容量 + 4bit模式指示位 + 8bit信息长度(版本10以及以上为16bit)
     */
    int dlen = capacity[QR_VERSION(qr->ver)][BYTE][qr->ec]+2+(qr->ver>=10?1:0);
    qr->data = (char*)malloc(dlen);
    memset(qr->data,0,dlen);
    int index = 0;
    qr->data[index] = 64; // 0100 0000 *0100是utf-8字节编码模式
    if (qr->ver >= 10) {
        qr->data[index] = (qr->data[index] & 0xf0) | ((len>>12)&0x0f);
        index++;
        qr->data[index++] = ((len&(0x0f<<8))>>4) | ((len&0xf0)>>4);
        qr->data[index] |= (len&0x0f)<<4;
    }else {
        qr->data[index++] |= ((char)len&0xf0)>>4;
        qr->data[index] |= ((char)len&0x0f)<<4;
    }

    for(int i = 0; i < len; i ++) {
        qr->data[index++] |= (bytes[i]&0xf0)>>4;
        qr->data[index] |= (bytes[i]&0x0f)<<4;
    }
    index++;
    bin("data",qr->data, index);
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
        qr->matrix[qr_XY(qr,cx,cy)] = 1;
        for(int j = 1; j<=3 ;j++) {
            int sx,sy;
            for ( sx = cx-j,sy=cy-j; sx<=cx+j; sx++,sy++) {
                qr->matrix[qr_XY(qr,sx,cy-j)] = j%2;
                qr->matrix[qr_XY(qr,sx,cy+j)] = j%2;
                qr->matrix[qr_XY(qr,cx+j,sy)] = j%2;
                qr->matrix[qr_XY(qr,cx-j,sy)] = j%2;
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
        qr->matrix[qr_XY(qr,7,i)] = 0;
        qr->matrix[qr_XY(qr,i,7)] = 0;

        qr->matrix[qr_XY(qr,i-8+qr->size,7)] = 0;
        qr->matrix[qr_XY(qr,qr->size-8,i)] = 0;

        qr->matrix[qr_XY(qr,i,qr->size-8)] = 0;
        qr->matrix[qr_XY(qr,7,qr->size-8+i)] = 0;
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
                if ( c == 255 ) {
                    qr->matrix[qr_XY(qr,cx,cy)] = 1;
                    for(int k = 1; k <= 2; k ++) {
                        int sx,sy;
                        for ( sx = cx-k,sy=cy-k; sx<=cx+k; sx++,sy++) {
                            qr->matrix[qr_XY(qr,sx,cy-k)] = k%2^1;
                            qr->matrix[qr_XY(qr,sx,cy+k)] = k%2^1;
                            qr->matrix[qr_XY(qr,cx+k,sy)] = k%2^1;
                            qr->matrix[qr_XY(qr,cx-k,sy)] = k%2^1;
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
        qr->matrix[qr_XY(qr,6,i)] = (i-7)%2;
        qr->matrix[qr_XY(qr,i,6)] = (i-7)%2;
    }
}

/*
 * 添加黑块跟信息区域 
 */
void qr_add_dark_reverse(QR qr)
{
    qr->matrix[qr_XY(qr,8,4*qr->ver+9)] = 1;
}

/*
 * 初始化qrcode 
 */
 QR qr_create( char * bytes,enum ec_level ec)
{
    int msg_length = strlen(bytes);
    int ver = 0;
    for (int i = 1; i<= QR_VERSION_MAX; i ++) {
        if(capacity[QR_VERSION(i)][BYTE][ec] >= msg_length) {
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
    qr->mode = BYTE;
    qr->size = QR_SIZE(ver);
    qr->matrix = (char *)malloc(qr->size*qr->size);
    for (int i=0;i<qr->size; i++) {
        for (int j=0;j<qr->size; j++) {
            qr->matrix[qr_XY(qr,i,j)] = 255;
        }
    }

    qr_encode_bytes(qr,bytes);
    qr_add_finder_pattern(qr);
    qr_add_separator(qr);
    qr_add_align_pattern(qr);
    qr_add_timing_pattern(qr);
    qr_add_dark_reverse(qr);
    return qr;
}

/*
 * 销毁一个qr 结构体
 */
void qr_destroy(QR qr)
{
    free(qr->matrix);
    free(qr);
}


