#include <stdio.h>
#include "qr.h"

int main (int argc, char * argv[])
{
	printf("hello qr code.\n");
	qr_t qr = qr_create("hello world",BYTE, ERROR_CORRECTION_H);
	qr_t qr2 = qr_create("一段很长的文本用来测试高版本的数据交错处理你好",BYTE, ERROR_CORRECTION_H);
	//qr_t qr3 = qr_create("https://www.thonky.com/qr-code-tutorial/how-create-generator-polynomial",BYTE, ERROR_CORRECTION_Q);
	return 0;
}
