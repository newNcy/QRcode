#include "qr.h"



int main (int argc, char **argv)
{

   const char * msg = argc<2?"hello":argv[1];
    QR qr = qr_create(msg,QR_EC_LEVEL_H);
    qr_view(qr);
     int len = qr_json_len(qr,msg);
     char * json = malloc(len);
    qr_to_json(qr,msg,json);
    qr_destroy(qr);

    printf("\n");
}
