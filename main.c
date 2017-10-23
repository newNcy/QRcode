#include "qr.h"



int main (int argc, char **argv)
{
    

    QR qr = qr_create(argc<2?"If the QR Code is version 7 or larger, you must include an 18-bit version information string in the bottom left and top right corners of the QR code. (For a full list of all possible version information strings, refer to the format and version tables.) The version information areas are the 6x3 blue rectangles shown in the images below. The version information is placed beside the finder patterns no matter how large the QR code is. The image on the left is a version 7 code, and the image on the right is a version 22 QR code.":argv[1],QR_EC_LEVEL_H);
    qr_view(qr);

    printf("\n");
}
