#include <stdio.h>
#include "qr.h"
#include "regonizer.h"

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

void set_pixel(byte_t * data, int w, int x, int y, byte_t r, byte_t g, byte_t b)
{
    byte_t * pr = data + y*w*3 + x*3;
    byte_t * pg = data + y*w*3 + x*3 +1;
    byte_t * pb = data + y*w*3 + x*3 +2;
    *pr = r;
    *pg = g;
    *pb = b;
}

/*
 * 熟悉的软光栅 bresenham
 */
void line(byte_t *data, int w, int x0, int y0, int x1, int y1, byte_t r, byte_t g, byte_t b)
{
    int dx = x1-x0;
    int dy = y1-y0;
    int steep = 0;
    if (abs(dy) > abs(dx)) {
        steep = 1;
        swap_int(x0, y0);
        swap_int(x1, y1);
        swap_int(dx, dy);
    }

    if (x0 > x1) {
        swap_int(x0, x1);
        swap_int(y0, y1);
    }

    float err = 0;
    float k = fabs((float)dy/dx);
    int y = y0;
    int step = y1 > y0 ?1:-1;
    assert(k < 1);
    for (int x = x0; x <= x1; ++ x) {
        if (steep) {
            set_pixel(data, w, y, x, r, g, b);
        }else {
            set_pixel(data, w, x, y, r, g, b);
        }
        err += k;
        if (err > 0.5) {
            y += step;
            err -= 1;
        }
    }
}
void rect(byte_t *data, int w, int x0, int y0, int x1, int y1, byte_t r, byte_t g, byte_t b) 
{
    line(data, w, x0,y0, x1, y0, r, g, b);
    line(data, w, x0,y0, x0, y1, r, g, b);

    line(data, w, x1,y1, x0, y1, r, g, b);
    line(data, w, x1,y0, x1, y1, r, g, b);
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
	byte_t * data = stbi_load("../qrs/e.jpg", &w, &h, &n, 0);
	int tw = 512, th = (int)(h * tw*1.0/w);
	byte_t * pro = (byte_t*)malloc(tw*th*n);
	byte_t * gray = (byte_t*)malloc(tw*th);
	byte_t * gray2 = (byte_t*)malloc(tw*th);
	byte_t * contour = (byte_t*)malloc(tw*th);
	byte_t * finder = (byte_t*)malloc(tw*th);
	stbir_resize_uint8(data, w, h, 0, pro, tw, th, 0, n);

	byte_t ma = 0, mi = 0xff;
	for (int y = 0; y < th; ++y) {
		for (int x = 0; x < tw; ++x) {
			byte_t * r = &pro[y*tw*3+x*3];
			byte_t * g = &pro[y*tw*3+x*3 + 1];
			byte_t * b = &pro[y*tw*3+x*3 + 2];
            byte_t avg = (*r*299 + *g*587+*b*114+500)/1000;
			gray[y*tw + x] = avg;
		}
	}

	stbi_write_png("gray.png", tw, th, 1, gray, tw);

    //中值滤波
    median_filter(gray, gray2, tw, th);
	stbi_write_png("gray2.png", tw, th, 1, gray2, tw);

	byte_t * bin = (byte_t*)malloc(tw*th);
	byte_t * bin2 = (byte_t*)malloc(tw*th);
    adaptive_thresholding(gray, bin, tw, th);
    adaptive_thresholding(gray2, bin2, tw, th);
	stbi_write_png("bin.png", tw, th, 1, bin, tw);
	stbi_write_png("bin2.png", tw, th, 1, bin2, tw);
    
    qr_regonizer_t regonizer;
    qr_regonizer_init(&regonizer);
    qr_regonizer_scan_finder_patterns(&regonizer, gray, tw, th);

    rect_t area = {{tw,th},{0,0}};
    printf("found finder %d patterns\n", regonizer.finder_patterns.size);
    for (int i = 0 ; i < regonizer.finder_patterns.size; ++i) {
        qr_finder_pattern_t * f = (qr_finder_pattern_t*)array_get(&regonizer.finder_patterns, i);
        int x = f->center.x;
        int y = f->center.y;
        area.min.x = min(area.min.x, f->min.x);
        area.min.y = min(area.min.y, f->min.y);
        area.max.x = max(area.max.x, f->max.x);
        area.max.y = max(area.max.y, f->max.y);
        int b = 1;
        set_pixel(pro, tw, x, y, 255, 0, 0);
        line(pro, tw, x-b, y, x+b, y, !f->center_dark*255, f->center_dark*255, 0);
        line(pro, tw, x, y-b, x, y+b, !f->center_dark*255, f->center_dark*255, 0);
        //rect(pro, tw, f->min[0], f->min[1], f->max[0], f->max[1], 0, 255, 0);
        //line(pro, tw, f->center[0], f->min[1], f->center[0], f->max[1], !f->center_dark*255, f->center_dark*255, 0);
        set_pixel(pro, tw, f->min.x, f->center.y, 0, 255, 0);
        set_pixel(pro, tw, f->max.x, f->center.y, 0, 255, 0);
        set_pixel(pro, tw, f->center.x, f->min.y, 0, 255, 0);
        set_pixel(pro, tw, f->center.x, f->max.y, 0, 255, 0);
        //line(pro, tw, f->min[0], f->center[1], f->max[0], f->center[1], !f->center_dark*255, f->center_dark*255, 0);
        //line(pro, tw, f->min[0], f->center[1], f->max[0], f->center[1], !f->center_dark*255, f->center_dark*255, 0);
    }

    //rect(pro, tw, area.min.x, area.min.y, area.max.x, area.max.y, 255,0,0);

    int count = regonizer.finder_patterns.size;
    int a = 0, b = 1, c = 2;
    int max_edge = 0;
    if (count == 3) {
        for (int i = 0 ; i < count ; ++i) {
            qr_finder_pattern_t * f = (qr_finder_pattern_t*)array_get(&regonizer.finder_patterns, i%count);
            qr_finder_pattern_t * f2 = (qr_finder_pattern_t*)array_get(&regonizer.finder_patterns, (i+1)%count);
            line(pro, tw, f->center.x, f->center.y, f2->center.x, f2->center.y, !f->center_dark*255, f->center_dark*255, 0);
            int dy = f2->center.y-f->center.y;
            int dx = f2->center.x-f->center.x;
            int edge = dy*dy + dx*dx;
            if (edge > max_edge) {
                max_edge = edge;
                c = i;
                b = (i+1)%count;
                a = (i+2)%count;
            }
        }
        qr_finder_pattern_t * f1 = (qr_finder_pattern_t*)array_get(&regonizer.finder_patterns, a);
        qr_finder_pattern_t * f2 = (qr_finder_pattern_t*)array_get(&regonizer.finder_patterns, b);
        qr_finder_pattern_t * f3 = (qr_finder_pattern_t*)array_get(&regonizer.finder_patterns, c);
        int dy = f3->center.y - f1->center.y;
        int dx = f3->center.x - f1->center.x;
        vec2_t down_right = {f2->center.x + dx, f2->center.y+dy};
        set_pixel(pro, tw, down_right.x, down_right.y, 0,0, 255);
        line(pro, tw, f2->center.x, f2->center.y, down_right.x, down_right.y, 0, 0, 255);
        line(pro, tw, f3->center.x, f3->center.y, down_right.x, down_right.y, 0, 0, 255);
        //再确定左下角 剩下的就是右上角了
        vec2_t up_left = f1->center;
        vec2_t down_left = f2->center, up_right = f3->center;
        vec2_t edge1 = vec2_sub(f2->center, up_left);
        vec2_t edge2 = vec2_sub(f3->center, up_left);
        if (vec2_cross(edge1, edge2) > 0) {
            down_left = f3->center;
            up_right = f2->center;
        }
        set_pixel(pro, tw, down_left.x, down_left.y, 255,0, 255);
    }
        adaptive_thresholding(gray, gray, tw, th);
	for (int y = 1; y < th; ++y) {
		for (int x = 1; x < tw; ++x) {
			byte_t * c = &gray[y*tw+x];
			byte_t * co = &contour[y*tw+x];
			if (*c != *(c-1) || *c != *(c-tw)) {
				*co = 255;
			}
		}
	}
    //hough_transform(contour, tw, th, pro);
	stbi_write_png("contour.png", tw, th, 1, contour, tw);
	stbi_write_png("finder.png", tw, th, 1, finder, tw);

	stbi_write_png("qr2.png", tw, th, 3, pro, tw*n);
	printf("%dx%d:%d\n", w, h, n);

	
	return 0;
}
