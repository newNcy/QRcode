#pragma once

#include "qr.h"
#include "array.h"

static inline int eq00(int a, int b, float d)
{
    double r = ((double)a)/b;
    return r >= 1-d && r <= 1+d;
}

static inline int eq0(int a, int b, float d)
{
    return eq00(a, b, d) || eq00(b,a,d);
}

static inline int eq1(int a, int b)
{
	return eq0(a, b, 0.5);
}

static inline int eq2(int a, int b)
{
	return eq0(a, b, 0.2);
}

struct vec2_t 
{
    int x, y;
};
typedef struct vec2_t vec2_t;

static inline vec2_t vec2_sub(vec2_t a, vec2_t b)
{
    vec2_t ret = {a.x-b.x, a.y - b.y};
    return ret;
}

static inline int vec2_cross(vec2_t a, vec2_t b)
{
    return a.x*b.y - a.y*b.x;
}

struct rect_t 
{
   vec2_t min, max;
};
typedef struct rect_t rect_t;

struct qr_finder_pattern_t
{
    vec2_t center;
    vec2_t min;
    vec2_t max;
    int err;
    int center_dark;
};

typedef struct qr_finder_pattern_t qr_finder_pattern_t;


struct qr_regonizer_t
{
    array_t finder_patterns;
};

typedef struct qr_regonizer_t  qr_regonizer_t;


typedef struct qr_finder_line qr_finder_line;

/*
 * 自适应阈值二值化
 */
void adaptive_thresholding(byte_t * in, byte_t * out, int w, int h);
/*
 * 中值滤波
 */
void median_filter(byte_t * in, byte_t * out, int w, int h);
/*
 * 高斯滤波
 */
void gaussion_filter(byte_t * in, byte_t * out, int w, int h);
/*
 * 霍夫变换
 */
void hough_transform(byte_t * in, int w, int h, byte_t * out);
/*
 * 腐蚀
 */
void erode(byte_t * in, byte_t * out, int w, int h);
/*
 * 膨胀
 */
void dilate(byte_t * in, byte_t * out, int w, int h);

void qr_regonizer_init(qr_regonizer_t * regonizer);
void qr_regonizer_push_finder_pattern(qr_regonizer_t * regonizer, qr_finder_pattern_t finder_pattern);
int qr_regonizer_scan_finder_patterns(qr_regonizer_t * regonizer, byte_t * bin, int w, int h);
qr_t qr_regonizer_find_qr(byte_t * data, int w, int h);

