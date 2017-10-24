#include "qr.h"



int main (int argc, char **argv)
{
    

    //QR qr = qr_create(argc<2?"If the QR Code is version 7 or larger, you must include an 18-bit version information string in the bottom left and top right corners of the QR code. (For a full list of all possible version information strings, refer to the format and version tables.) The version information areas are the 6x3 blue rectangles shown in the images below. The version information is placed beside the finder patterns no matter how large the QR code is. The image on the left is a version 7 code, and the image on the right is a version 22 QR code.":argv[1],QR_EC_LEVEL_L);
    //QR qr = qr_create("BEGIN:VCARD\nVERSION:2.1\nN;CHARSET=UTF-8;ENCODING=QUOTED-PRINTABLE:=E9=99=88;=E7=A7=8B=E5=AE=87;;;\nFN;CHARSET=UTF-8;ENCODING=QUOTED-PRINTABLE:=E9=99=88=E7=A7=8B=E5=AE=87\nTEL;CELL;PREF:13667832690\nCATEGORIES;CHARSET=UTF-8;ENCODING=QUOTED-PRINTABLE:=E5=AE=B6=E4=BA=BA\nEND:VCARD\n",QR_EC_LEVEL_M);
    QR qr = qr_create(argc>2?"hello":argv[1],QR_EC_LEVEL_M);
    qr_view(qr);
    qr_destroy(qr);

    printf("\n");
}
