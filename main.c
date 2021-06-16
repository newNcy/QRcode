#include <stdio.h>
#include "qr.h"

int main (int argc, char * argv[])
{
	printf("hello qr code.\n");
	qr_t qr = qr_create("hello world",BYTE, ERROR_CORRECTION_M);
	qr_t qr2 = qr_create("一段很长的文本，用来测试高版本的数据交错处理",BYTE, ERROR_CORRECTION_H);
	return 0;
}
