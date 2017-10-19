#include "qr.h"



int main (int argc, char **argv)
{
    
    //gf_view(&gf);

    int cap = capacity[QR_VERSION(1)][BYTE][Q];
    int elen = 10;
    char g[10] = {0};
    char ec[10] = {0};
    /*
    gf_make_generator_poly(&gf,g,elen);
    reed_solomon(&gf,mpoly,0,16,ec,10,g);

    poly_view("gen",g,10);
    poly_view("dc",mpoly,16);
    poly_view("ec",ec,10);
    */

    QR qr = qr_create(argc<2?"hello world":argv[1],Q);
    qr_view(qr);

    printf("\n");
}
