#include <stdio.h>
#include "qr.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"


int foo(double a, double b, int r)
{
	return fabs((double)a/(double)b - r) < 0.1;
}

int eq(int a, int b)
{
	return fabs(a*1.0-b*1.0) < 0.1;
}

int main (int argc, char * argv[])
{
	/*
	printf("hello qr code.\n");
	qr_t qr2 = qr_create("一段很长的文本用来测试高版本的数据交错处理",BYTE, ERROR_CORRECTION_H);
	qr_t qr3 = qr_create("，以及村子周围密布着不同方向的堰堤。河流以这样的方式重塑自己的骨骼，健壮村庄的腰身，它需要一条能容纳自己身躯的宽阔的河道，需要一个能容纳自由奔腾呼啸向前的道场，需要一直向前奔涌和畅通无阻的健壮通道，它能带来新生和温情，也能摧毁土地和生灵，这让鸡龙河虽然在刘家庄断流了，却在几乎窒息的重压下，能重整旗鼓，坚忍不拔执着无悔地一往无前，十分潇洒地挂在我村子的左肩膀上流淌千年。",BYTE, ERROR_CORRECTION_H);
	fflush(stdout);
	//qr_t qr3 = qr_create("https://www.thonky.com/qr-code-tutorial/how-create-generator-polynomial",BYTE, ERROR_CORRECTION_Q);
 
	qr_t qr = qr_create("hello world",BYTE, ERROR_CORRECTION_M);
	int w = 512, h = 512, c;
	byte_t * data = (byte_t*)malloc(w * h);
	for (int i = 0; i < h; ++ i) {
		for (int j = 0; j < w; ++ j) {
			int y = 1.0*i/h*qr.size;
			int x = 1.0*j/w*qr.size;
			data[i*w+ j] = !qr.modules[y*qr.size+x] * 255;
		}
	}
	stbi_write_png("qr.png", w, h, 1, data, w);
	free(data);
	qr_destroy(qr);
	*/

	int w, h, n;
	byte_t * data = stbi_load("../qrs/b.jpg", &w, &h, &n, 0);
	int tw = 512, th = h * tw/w;
	byte_t * pro = (byte_t*)malloc(tw*th*n);
	byte_t * gray = (byte_t*)malloc(tw*th);
	byte_t * contour = (byte_t*)malloc(tw*th);
	stbir_resize_uint8(data, w, h, 0, pro, tw, th, 0, n);

	byte_t ma = 0, mi = 0xff;
	for (int y = 0; y < th; ++y) {
		for (int x = 0; x < tw; ++x) {
			byte_t * r = &pro[y*tw*3+x*3];
			byte_t * g = &pro[y*tw*3+x*3 + 1];
			byte_t * b = &pro[y*tw*3+x*3 + 2];
			byte_t avg = ((int)*r+*g+ *b)/3;
			gray[y*tw + x] = avg;
			ma = max(ma, avg);
			mi = min(mi, avg);
		}
	}

	byte_t threshold = (mi + ma)/2;
	for (int y = 0; y < th; ++y) {
		for (int x = 0; x < tw; ++x) {
			byte_t * c = gray + y*tw + x;
			if (*c > threshold) {
				*c = 255;
			}else {
				*c = 0;
			}
		}
	}

	for (int y = 1; y < th; ++y) {
		for (int x = 1; x < tw; ++x) {
			byte_t * c = &gray[y*tw+x];
			byte_t * co = &contour[y*tw+x];
			if (*c != *(c-1) || *c != *(c-tw)) {
				*co = 255;
			}
		}
	}

	for (int y = 1; y < th; ++y) {
		int rec [5] = {0, 0, 0, 0, 1};
		int start []= {0, 0, 0,0,0};
		fflush(stdout);
		int state = 0;
		byte_t pre = gray[y*tw+0];
		for (int x = 1; x < tw; ++x) {
			byte_t * r = &pro[y*tw*3+x*3];
			byte_t * g = &pro[y*tw*3+x*3 + 1];
			byte_t * b = &pro[y*tw*3+x*3 + 2];
			byte_t * c = gray + y*tw + x;
			if (*c == pre)  {
				rec[4] ++;
				/*
				if (state == 4) {
					*r = 255;
				} else if (state == 3) {
					*g = 255;
				} else if (state == 2) {
					*b = 255;
					*g = 0;
					*r = 0;
				} else if (state == 1) {
					*b = 255;
					*r = 255;
				} else if (state == 0) {
					*r = 255;
					*g = 255;
				} else if (state == 5) {
					*g = 255;
				}
				*/
			} else {
				state ++; 
				pre = *c;
				for (int i = 0; i < 4; ++i) {
					rec[i] = rec[i+1];
					start[i] = start[i+1];
				}
				start[4] = x;
				rec[4] = 1;
				
			}
			if (state > 2) {
				if (eq(rec[0], rec[1]) && eq(rec[1],rec[3]) && eq(rec[3], rec[4]) && eq(rec[0]*3, rec[2])) {
					printf("%d %d %d %d %d\n", rec[0], rec[1],rec[2],rec[3],rec[4]);
					*r = 255;
				}
			}

			
		}
	}
	stbi_write_png("qr.png", tw, th, 1, gray, tw);
	stbi_write_png("qr2.png", tw, th, 3, pro, tw*n);
	stbi_write_png("contour.png", tw, th, 1, contour, tw);
	printf("%dx%d:%d\n", w, h, n);

	
	return 0;
}
