#include <stdio.h>
#include "qr.h"

int main (int argc, char * argv[])
{
	printf("hello qr code.\n");
	qr_t qr = qr_create("hello world",BYTE, ERROR_CORRECTION_H);
	qr_t qr2 = qr_create("一段很长的文本用来测试高版本的数据交错处理",BYTE, ERROR_CORRECTION_H);
	qr_t qr3 = qr_create("，以及村子周围密布着不同方向的堰堤。河流以这样的方式重塑自己的骨骼，健壮村庄的腰身，它需要一条能容纳自己身躯的宽阔的河道，需要一个能容纳自由奔腾呼啸向前的道场，需要一直向前奔涌和畅通无阻的健壮通道，它能带来新生和温情，也能摧毁土地和生灵，这让鸡龙河虽然在刘家庄断流了，却在几乎窒息的重压下，能重整旗鼓，坚忍不拔执着无悔地一往无前，十分潇洒地挂在我村子的左肩膀上流淌千年。",BYTE, ERROR_CORRECTION_H);
	fflush(stdout);
	//qr_t qr3 = qr_create("https://www.thonky.com/qr-code-tutorial/how-create-generator-polynomial",BYTE, ERROR_CORRECTION_Q);
	return 0;
}
