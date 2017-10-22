#include "qr.h"



int main (int argc, char **argv)
{
    

    QR qr = qr_create(argc<2?"快速响应二维码测试":argv[1],Q);
    qr_view(qr);

    printf("\n");
}
