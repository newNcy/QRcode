#pragma once
#include <stddef.h>
typedef unsigned char byte_t;
#ifndef __cplusplus
#define nullptr NULL
#endif

#define min(a, b) (a > b? b:a)
#define max(a, b) (a < b? b:a)
#define swap_int(a, b) {int t = a; a = b; b = t;}


