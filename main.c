#include <stdio.h>
#include "qr.h"

int main (int argc, char * argv[])
{
	printf("hello qr code.\n");
	qr_t qr = qr_create("hello there world",BYTE, ERROR_CORRECTION_Q);
	return 0;
}
