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

/*
 * 高斯消元法解线性方程组
 */
void gauss(double * ab, double *res, int len)
{
	for (int i = 0; i < len; ++ i) {
		printf("%g*X%d", ab[i*(len+1)], len);
		for (int j = 1; j < len; ++ j) {
			printf(" + %g*X%d", ab[i*(len+1)+j], len-i);
		}
		printf(" = %g\n", ab[i*(len+1)+len]);
	}
	for (int i = 0; i < len; ++ i) {
		for (int j = 0; j < i; ++ j) {
			double lead = ab[i*(len+1)+j];
			if (lead != 0) {
				for (int k = 0; k < len+1; ++k) {
					ab[i * (len+1)+k] -= ab[j*(len+1)+k]*lead;
				}
			}
		}

		double lead = ab[i * (len+1)+i];
		if (lead != 0) {
			for (int j = i; j < len+1; ++ j) {
				ab[i *(len+1)+ j] /= lead;
			}
		}
	}

	//回代
	for (int i = len-1; i >=0; --i) {
		//用第j行带入式子i
		res[i] = ab[i*(len+1)+len];
		for (int j = i+1; j < len; ++j) {
			res[i] -= res[j] * ab[i*(len+1) +j];
		}
		printf("x%d = %f\n", i, res[i]);
	}

}

/*
 */
void inverse(double mat[3][3], double out[3][3])
{
    //double det = mat[0][0] * (mat[1][1] * mat[2][2] - mat[2][1]*mat[1][2]) - mat[0][1]*(mat[1][2]*mat[2][0]-mat[2][1]*mat[1][0]) + mat[0][2]*(mat[1][0]*mat[2][1]-mat[2][0]*mat[1][1]);
    double det = mat[0][0]*mat[1][1]*mat[2][2] + mat[0][1]*mat[1][2]*mat[2][0] + mat[0][2]*mat[1][0]*mat[2][1];
    det -= mat[0][0]*mat[1][2]*mat[2][1] + mat[0][1]*mat[1][0]*mat[2][2] + mat[0][2]*mat[1][1]*mat[2][0];

    double det_1 = 1/det;
    double c[3][3] =
    {
        {mat[1][1] * mat[2][2]-mat[1][2]*mat[2][1], mat[0][1] * mat[2][2]-mat[1][2]*mat[2][1], mat[1][1] * mat[2][2]-mat[1][2]*mat[2][1]},
    };
    printf("%f\n", det);
}

void get_perspective_transform(vec2_t s[4], vec2_t d[4], double * res)
{
	double ab [] = 
	{
		s[0].x, s[0].y, 1, 0,0,0, -s[0].x*d[0].x, -s[0].y*d[0].x, d[0].x,
		s[1].x, s[1].y, 1, 0,0,0, -s[1].x*d[1].x, -s[1].y*d[1].x, d[1].x,
		s[2].x, s[2].y, 1, 0,0,0, -s[2].x*d[2].x, -s[2].y*d[2].x, d[2].x,
		s[3].x, s[3].y, 1, 0,0,0, -s[3].x*d[3].x, -s[3].y*d[3].x, d[3].x,
		
		0,0,0, s[0].x, s[0].y, 1, -s[0].x*d[0].y, -s[0].y*d[0].y, d[0].y,
		0,0,0, s[1].x, s[1].y, 1, -s[1].x*d[1].y, -s[1].y*d[1].y, d[1].y,
		0,0,0, s[2].x, s[2].y, 1, -s[2].x*d[2].y, -s[2].y*d[2].y, d[2].y,
		0,0,0, s[3].x, s[3].y, 1, -s[3].x*d[3].y, -s[3].y*d[3].y, d[3].y
	};
	gauss(ab, res, 8);
}

int main (int argc, char * argv[])
{
	printf("hello qr code.\n");
	//qr_t qr3 = qr_create("https://www.thonky.com/qr-code-tutorial/how-create-generator-polynomial",BYTE, ERROR_CORRECTION_Q);
 
	/*
	qr_t qr = qr_create("hello world 你好",BYTE, ERROR_CORRECTION_L);
	fflush(stdout);
	qr_destroy(qr);
	*/

	int w, h, n;
	byte_t * data = stbi_load("../qrs/a.jpg", &w, &h, &n, 0);
	int max_w = 512;
	int tw = w>max_w?max_w:w, th = (int)(h * tw*1.0/w);
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

		vec2_t src[] = 
		{
			up_left, up_right, down_right, down_left
		};
		vec2_t dst[] = 
		{
			{tw/4, th/4},
			{tw/4*3, th/4},
			{tw/4*3, th/4*3},
			{tw/4, th/4*3},
		};
		double c[9] = {0};
        c[8] = 1;
        double c2[9] = {0.79, -0.24, 8378, 0.828, 4.36, 2152, 0.00041, -0.00025, 26.0338};
		get_perspective_transform(dst, src, c);
        inverse(c, c2);
		byte_t * trans = (byte_t*)malloc(tw*th);
        memset(trans, 0, tw*th);
		for (int v = 0; v < th; ++ v) {
			for (int u = 0; u < tw; ++ u) {
				int x = (c[0]*u+c[1]*v+c[2])/(c[6]*u + c[7]*v +c[8]);
				int y = (c[3]*u+c[4]*v+c[5])/(c[6]*u + c[7]*v +c[8]);
				if (x>=0 && x < tw && y >= 0 && y < th) {
					trans[v*tw+u] = bin[y*tw+x];
				}
			}
		}
		stbi_write_png("trans.png", tw, th, 1, trans, tw);
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

	
	double ab[] =
	{
		5, 2, 1, -12,
		-1, 4, 2, 20,
		2, -3, 10, 3 
	};
	double res[3] = {0};

	gauss(ab, res, 3);
	return 0;
}
