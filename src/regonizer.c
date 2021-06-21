#include "regonizer.h"
#include "math.h"
#include <malloc.h>
#include <stdio.h>

const static float pi = 3.1415926;
void adaptive_thresholding(byte_t * in, byte_t * out, int w, int h)
{
    int * intImg = (int*)malloc(w*h*sizeof(int));
    int s = w/10;
    int t = 25; //percent
    for (int i = 0;i < w; ++ i) {
        int sum = 0;
        for (int j = 0; j < h; ++ j) {
            sum = sum + in[j*w +i];
            if (j == 0) {
                intImg[j*w +i] = sum;
            }else if (j*w+i-1>=0) {
                intImg[j*w +i] = intImg[j*w+i-1] + sum;
            }
        }
    }
    for (int i = 0; i < w; ++ i) {
        for (int j = 0; j < h; ++ j) {
            //窗口bounding
            int x1 = max(i - s/2, 0);
            int x2 = min(i + s/2, w-1);
            int y1 = max(j - s/2, 0);
            int y2 = min(j + s/2, h-1);

            //窗口面积
            int count = (x2-x1) * (y2-y1); 
            int sum = intImg[y2*w+x2] - intImg[y1*w+x2] - intImg[y2*w+x1] + intImg[y1*w + x1];

            if (in[j*w+i] * count <= sum*(100-t)/100.0) {
                out[j*w +i] = 0;
            } else {
                out[j*w +i] = 255;
            }
        }
    }

}

void median_filter(byte_t * in, byte_t * out, int w, int h)
{
    int kernel = 3;
    int border = kernel/2;
    int count = 9;
    int win[9] = {0};
    for (int y = border; y < h-border; ++ y) {
        for (int x = border; x < w-border; ++ x) {

            for (int dy = -border; dy <= border; ++ dy) {
                for (int dx = -border; dx <= border; ++ dx) {
                    win[(dy+border)*kernel + dx+border] = in[(y+dy)*w + x + dx];
                }
            }
            //sort win
            for (int i = 0; i < count-1; ++ i) {
                for (int j = 0; j < count-1-i; ++ j) {
                    if (win[j] > win[j+1]) {
                        int t = win[j];
                        win[j] = win[j+1];
                        win[j+1] = t;
                    }
                }
            }
            out[y*w+x] = win[count/2];
        }
    }
}

void hough_transform(byte_t * in, int w, int h, byte_t * out)
{
    int * count[360] = {0};
    int dis = sqrt(w*w+h*h);
    int maxrho = 2*dis;
    for (int i = 0; i < 180; ++i) {
        count[i] = (int*)malloc(maxrho*sizeof(int));
        memset(count[i], 0, maxrho*sizeof(int));
    }

    for (int theta = 0; theta < 180; ++theta) {
        float d = theta/180.0*pi;
        float C = cos(d);
        float S = sin(d);
        for (int y = 0; y < h; ++ y) {
            for (int x = 0; x < w; ++ x) {
                if (in[y*w +x]) {
                    float p = x*C + y*S;
                    if (p+dis >= 0){
                        count[theta][(int)(p+dis)] ++;
                    }
                }
            }
        }
    }
    int line = 0;
    int rtheta = 0;
    int rp = 0;
    int c = 0;
    for (int theta = 0; theta < 180; ++theta) {
        for (int p = 0; p < maxrho; ++p) {
            if (count[theta][p] > 200) {
                if (count[theta][p] > c) {
                    c = count[theta][p];
                    rtheta = theta;
                    rp = p;
                    
                }
                float d = theta/180.0*pi;
                    float C = cos(d);
                    float S = sin(d);
                    for (int x = 0; x < w; ++ x) {
                        float y = (p-dis-x*C)/S;
                        if (y >=0 && y < h) {
                            out[(int)y*w*3+x*3] = 255;
                        }
                    }
                    for (int y = 0; y < h; ++ y) {
                        float x = (p-dis-y*S)/C;
                        if (x >=0 && x < w) {
                            out[(int)y*w*3+(int)x*3] = 255;
                        }
                    }
                line ++;
            }
        }
    }
    printf("line %d %d %d\n", line, rtheta, rp);
    //p = x*cos(theta) + y*sin(theta);
    //y = (p - x*cos(theta))/sin(theta)

    fflush(stdout);
}

void erode(byte_t * in, byte_t * out, int w, int h)
{
}


qr_t qr_regonizer_find_qr(byte_t * gray, int w, int h)
{

    
}

void qr_regonizer_init(qr_regonizer_t * regonizer)
{
    array_init(&regonizer->finder_patterns, sizeof(qr_finder_pattern_t));
}

int check_finder_pattern(int r[])
{
	int check1 = eq1(r[0], r[1]) && eq1(r[1], r[3]) && eq1(r[3],r[4]) && eq1(r[0], r[4]);
	int check3 = eq1(r[2], 3*r[0])&& eq1(r[2], 3*r[1])&& eq1(r[2], 3*r[3])&& eq1(r[2], 3*r[4]);
	int check3_2 = !eq1(r[2], r[0]) && !eq1(r[2], r[1]) && !eq1(r[2], r[3]) && !eq1(r[2], r[4]);
	int check5 = eq1(r[2] + r[1]+r[3], 5*r[0]);
	int check7 = eq1(r[2] + r[1]+r[3] + r[0]+r[4], 7*r[0]);
	return check1 && check3 && check3_2 && check5 && check7;
}


int qr_regonizer_scan_finder_patterns(qr_regonizer_t * regonizer, byte_t * gray, int w, int h)
{
    byte_t * bin = (byte_t*)malloc(w*h);
    adaptive_thresholding(gray, bin, w, h);
    for (int y = 1; y < h; ++ y) {
		int r[5] = {0, 0, 0, 0, 1}; //record
		int start []= {0, 0, 0,0,0};
		int state = 0;
        int q = 0;
		byte_t pre = bin[y*w+0];
        for (int x = 1; x < w; ++ x) {
            byte_t * c = bin+ y*w + x;
            if (*c != pre) {
                state ++;

                if (state > 4 && check_finder_pattern(r)) {
                //横向发现符合比例，则进行竖向扫描
                    byte_t * center = bin + y*w+(start[0]+x-1)/2;
                    byte_t t = *center;
                    byte_t * p = center - w;
                    //两个方向的计数
					int vr[5] = {0, 0, 1, 0, 0};
                    int idx = 2;
                    int y0 = y, y1 = y;
                    while (p > bin && idx < 5) {
                        if (*p != t) {
                            idx ++;
                            t = *p;
                        }
                        p = p - w;
                        vr[idx] ++;
                        y0 --;
                    }

					//记录上半部分宽度
					int up_part = vr[2];
                    //reset
                    t = *center;
                    p = center + w;
                    idx = 2;
                    while (p < bin+w*h && idx >=0) {
                        if (*p != t) {
                            idx --; 
                            t = *p;
                        }
                        p = p + w;
                        vr[idx] ++;
                        y1 ++;
                    }
                    y0 ++;
                    y1 --;

                    //检查竖向是否符合比例
                    
                    if (check_finder_pattern(vr) && eq2(up_part, vr[2]-up_part)) {
                        int center_x = (x+start[0]-1)/2;
                        qr_finder_pattern_t finder_pattern = 
                        {
                            {center_x, y},
                            {start[0] -1, y0},
                            {x-1, y1},
                            abs(vr[2]-2*up_part),
                            !pre,
                        };
                        int drop = 0;
                        for (int i = 0; i < regonizer->finder_patterns.size; ++i) {
                            qr_finder_pattern_t * f = (qr_finder_pattern_t*)array_get(&regonizer->finder_patterns, i);
                            //如果跟现有的点离得太近则选择误差小的,具体误差后面可以优化,现在先纯看是不是中心块的中心
                            if (abs(f->center.x-center_x) < r[2] && abs(f->center.y - y) < r[2]) {
                                if (f->err > finder_pattern.err) {
                                    *f = finder_pattern;
                                } 
                                drop = 1;
                            }
                        }
                        if (!drop) {
                            
                            array_push(&regonizer->finder_patterns, &finder_pattern);
                        }
                    }
                }
                for (int i = 0; i < 4; ++i) {
                    r[i] = r[i+1];
                    start[i] = start[i+1];
                }
                r[4] = 1;
                start[4] = x;
                pre = *c;
            }else {
                r[4] ++;
            }
        }
    }
    free(bin);
}
