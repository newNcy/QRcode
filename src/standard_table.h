#pragma once


/* gen by make_constants.py */


/* version < EC_level < Mode */
/* Mode : Num AlphaNum Byte Kanji */
static int qr_capacities[40][4][4] = 
{
    { //v1 Num AlphaNum Byte Kanji
        { 41,25,17,10,}, // L
        { 34,20,14,8,}, // M
        { 27,16,11,7,}, // Q
        { 17,10,7,4,}, // H
    },
    { //v5 Num AlphaNum Byte Kanji
        { 77,47,32,20,}, // L
        { 63,38,26,16,}, // M
        { 48,29,20,12,}, // Q
        { 34,20,14,8,}, // H
    },
    { //v9 Num AlphaNum Byte Kanji
        { 127,77,53,32,}, // L
        { 101,61,42,26,}, // M
        { 77,47,32,20,}, // Q
        { 58,35,24,15,}, // H
    },
    { //v13 Num AlphaNum Byte Kanji
        { 187,114,78,48,}, // L
        { 149,90,62,38,}, // M
        { 111,67,46,28,}, // Q
        { 82,50,34,21,}, // H
    },
    { //v17 Num AlphaNum Byte Kanji
        { 255,154,106,65,}, // L
        { 202,122,84,52,}, // M
        { 144,87,60,37,}, // Q
        { 106,64,44,27,}, // H
    },
    { //v21 Num AlphaNum Byte Kanji
        { 322,195,134,82,}, // L
        { 255,154,106,65,}, // M
        { 178,108,74,45,}, // Q
        { 139,84,58,36,}, // H
    },
    { //v25 Num AlphaNum Byte Kanji
        { 370,224,154,95,}, // L
        { 293,178,122,75,}, // M
        { 207,125,86,53,}, // Q
        { 154,93,64,39,}, // H
    },
    { //v29 Num AlphaNum Byte Kanji
        { 461,279,192,118,}, // L
        { 365,221,152,93,}, // M
        { 259,157,108,66,}, // Q
        { 202,122,84,52,}, // H
    },
    { //v33 Num AlphaNum Byte Kanji
        { 552,335,230,141,}, // L
        { 432,262,180,111,}, // M
        { 312,189,130,80,}, // Q
        { 235,143,98,60,}, // H
    },
    { //v37 Num AlphaNum Byte Kanji
        { 652,395,271,167,}, // L
        { 513,311,213,131,}, // M
        { 364,221,151,93,}, // Q
        { 288,174,119,74,}, // H
    },
    { //v41 Num AlphaNum Byte Kanji
        { 772,468,321,198,}, // L
        { 604,366,251,155,}, // M
        { 427,259,177,109,}, // Q
        { 331,200,137,85,}, // H
    },
    { //v45 Num AlphaNum Byte Kanji
        { 883,535,367,226,}, // L
        { 691,419,287,177,}, // M
        { 489,296,203,125,}, // Q
        { 374,227,155,96,}, // H
    },
    { //v49 Num AlphaNum Byte Kanji
        { 1022,619,425,262,}, // L
        { 796,483,331,204,}, // M
        { 580,352,241,149,}, // Q
        { 427,259,177,109,}, // H
    },
    { //v53 Num AlphaNum Byte Kanji
        { 1101,667,458,282,}, // L
        { 871,528,362,223,}, // M
        { 621,376,258,159,}, // Q
        { 468,283,194,120,}, // H
    },
    { //v57 Num AlphaNum Byte Kanji
        { 1250,758,520,320,}, // L
        { 991,600,412,254,}, // M
        { 703,426,292,180,}, // Q
        { 530,321,220,136,}, // H
    },
    { //v61 Num AlphaNum Byte Kanji
        { 1408,854,586,361,}, // L
        { 1082,656,450,277,}, // M
        { 775,470,322,198,}, // Q
        { 602,365,250,154,}, // H
    },
    { //v65 Num AlphaNum Byte Kanji
        { 1548,938,644,397,}, // L
        { 1212,734,504,310,}, // M
        { 876,531,364,224,}, // Q
        { 674,408,280,173,}, // H
    },
    { //v69 Num AlphaNum Byte Kanji
        { 1725,1046,718,442,}, // L
        { 1346,816,560,345,}, // M
        { 948,574,394,243,}, // Q
        { 746,452,310,191,}, // H
    },
    { //v73 Num AlphaNum Byte Kanji
        { 1903,1153,792,488,}, // L
        { 1500,909,624,384,}, // M
        { 1063,644,442,272,}, // Q
        { 813,493,338,208,}, // H
    },
    { //v78 Num AlphaNum Byte Kanji
        { 2061,1249,858,528,}, // L
        { 1600,970,666,410,}, // M
        { 1159,702,482,297,}, // Q
        { 919,557,382,235,}, // H
    },
    { //v82 Num AlphaNum Byte Kanji
        { 2232,1352,929,572,}, // L
        { 1708,1035,711,438,}, // M
        { 1224,742,509,314,}, // Q
        { 969,587,403,248,}, // H
    },
    { //v86 Num AlphaNum Byte Kanji
        { 2409,1460,1003,618,}, // L
        { 1872,1134,779,480,}, // M
        { 1358,823,565,348,}, // Q
        { 1056,640,439,270,}, // H
    },
    { //v90 Num AlphaNum Byte Kanji
        { 2620,1588,1091,672,}, // L
        { 2059,1248,857,528,}, // M
        { 1468,890,611,376,}, // Q
        { 1108,672,461,284,}, // H
    },
    { //v94 Num AlphaNum Byte Kanji
        { 2812,1704,1171,721,}, // L
        { 2188,1326,911,561,}, // M
        { 1588,963,661,407,}, // Q
        { 1228,744,511,315,}, // H
    },
    { //v98 Num AlphaNum Byte Kanji
        { 3057,1853,1273,784,}, // L
        { 2395,1451,997,614,}, // M
        { 1718,1041,715,440,}, // Q
        { 1286,779,535,330,}, // H
    },
    { //v102 Num AlphaNum Byte Kanji
        { 3283,1990,1367,842,}, // L
        { 2544,1542,1059,652,}, // M
        { 1804,1094,751,462,}, // Q
        { 1425,864,593,365,}, // H
    },
    { //v106 Num AlphaNum Byte Kanji
        { 3517,2132,1465,902,}, // L
        { 2701,1637,1125,692,}, // M
        { 1933,1172,805,496,}, // Q
        { 1501,910,625,385,}, // H
    },
    { //v110 Num AlphaNum Byte Kanji
        { 3669,2223,1528,940,}, // L
        { 2857,1732,1190,732,}, // M
        { 2085,1263,868,534,}, // Q
        { 1581,958,658,405,}, // H
    },
    { //v114 Num AlphaNum Byte Kanji
        { 3909,2369,1628,1002,}, // L
        { 3035,1839,1264,778,}, // M
        { 2181,1322,908,559,}, // Q
        { 1677,1016,698,430,}, // H
    },
    { //v118 Num AlphaNum Byte Kanji
        { 4158,2520,1732,1066,}, // L
        { 3289,1994,1370,843,}, // M
        { 2358,1429,982,604,}, // Q
        { 1782,1080,742,457,}, // H
    },
    { //v122 Num AlphaNum Byte Kanji
        { 4417,2677,1840,1132,}, // L
        { 3486,2113,1452,894,}, // M
        { 2473,1499,1030,634,}, // Q
        { 1897,1150,790,486,}, // H
    },
    { //v126 Num AlphaNum Byte Kanji
        { 4686,2840,1952,1201,}, // L
        { 3693,2238,1538,947,}, // M
        { 2670,1618,1112,684,}, // Q
        { 2022,1226,842,518,}, // H
    },
    { //v130 Num AlphaNum Byte Kanji
        { 4965,3009,2068,1273,}, // L
        { 3909,2369,1628,1002,}, // M
        { 2805,1700,1168,719,}, // Q
        { 2157,1307,898,553,}, // H
    },
    { //v134 Num AlphaNum Byte Kanji
        { 5253,3183,2188,1347,}, // L
        { 4134,2506,1722,1060,}, // M
        { 2949,1787,1228,756,}, // Q
        { 2301,1394,958,590,}, // H
    },
    { //v138 Num AlphaNum Byte Kanji
        { 5529,3351,2303,1417,}, // L
        { 4343,2632,1809,1113,}, // M
        { 3081,1867,1283,790,}, // Q
        { 2361,1431,983,605,}, // H
    },
    { //v142 Num AlphaNum Byte Kanji
        { 5836,3537,2431,1496,}, // L
        { 4588,2780,1911,1176,}, // M
        { 3244,1966,1351,832,}, // Q
        { 2524,1530,1051,647,}, // H
    },
    { //v146 Num AlphaNum Byte Kanji
        { 6153,3729,2563,1577,}, // L
        { 4775,2894,1989,1224,}, // M
        { 3417,2071,1423,876,}, // Q
        { 2625,1591,1093,673,}, // H
    },
    { //v150 Num AlphaNum Byte Kanji
        { 6479,3927,2699,1661,}, // L
        { 5039,3054,2099,1292,}, // M
        { 3599,2181,1499,923,}, // Q
        { 2735,1658,1139,701,}, // H
    },
    { //v154 Num AlphaNum Byte Kanji
        { 6743,4087,2809,1729,}, // L
        { 5313,3220,2213,1362,}, // M
        { 3791,2298,1579,972,}, // Q
        { 2927,1774,1219,750,}, // H
    },
    { //v158 Num AlphaNum Byte Kanji
        { 7089,4296,2953,1817,}, // L
        { 5596,3391,2331,1435,}, // M
        { 3993,2420,1663,1024,}, // Q
        { 3057,1852,1273,784,}, // H
    },
};

typedef struct
{
    int ec_codeword_per_block;
    int group1_blocks;
    int data_codeword_per_block1;
    int group2_blocks;
    int data_codeword_per_block2;
}qr_error_correction_parameter_t;

static qr_error_correction_parameter_t qr_error_correction_parameter [40][4] =
{
    { //v1
        {  7,  1, 19,  0,  0},// level L
        { 10,  1, 16,  0,  0},// level M
        { 13,  1, 13,  0,  0},// level Q
        { 17,  1,  9,  0,  0} // level H
    },
    { //v2
        { 10,  1, 34,  0,  0},// level L
        { 16,  1, 28,  0,  0},// level M
        { 22,  1, 22,  0,  0},// level Q
        { 28,  1, 16,  0,  0} // level H
    },
    { //v3
        { 15,  1, 55,  0,  0},// level L
        { 26,  1, 44,  0,  0},// level M
        { 18,  2, 17,  0,  0},// level Q
        { 22,  2, 13,  0,  0} // level H
    },
    { //v4
        { 20,  1, 80,  0,  0},// level L
        { 18,  2, 32,  0,  0},// level M
        { 26,  2, 24,  0,  0},// level Q
        { 16,  4,  9,  0,  0} // level H
    },
    { //v5
        { 26,  1,108,  0,  0},// level L
        { 24,  2, 43,  0,  0},// level M
        { 18,  2, 15,  2, 16},// level Q
        { 22,  2, 11,  2, 12} // level H
    },
    { //v6
        { 18,  2, 68,  0,  0},// level L
        { 16,  4, 27,  0,  0},// level M
        { 24,  4, 19,  0,  0},// level Q
        { 28,  4, 15,  0,  0} // level H
    },
    { //v7
        { 20,  2, 78,  0,  0},// level L
        { 18,  4, 31,  0,  0},// level M
        { 18,  2, 14,  4, 15},// level Q
        { 26,  4, 13,  1, 14} // level H
    },
    { //v8
        { 24,  2, 97,  0,  0},// level L
        { 22,  2, 38,  2, 39},// level M
        { 22,  4, 18,  2, 19},// level Q
        { 26,  4, 14,  2, 15} // level H
    },
    { //v9
        { 30,  2,116,  0,  0},// level L
        { 22,  3, 36,  2, 37},// level M
        { 20,  4, 16,  4, 17},// level Q
        { 24,  4, 12,  4, 13} // level H
    },
    { //v10
        { 18,  2, 68,  2, 69},// level L
        { 26,  4, 43,  1, 44},// level M
        { 24,  6, 19,  2, 20},// level Q
        { 28,  6, 15,  2, 16} // level H
    },
    { //v11
        { 20,  4, 81,  0,  0},// level L
        { 30,  1, 50,  4, 51},// level M
        { 28,  4, 22,  4, 23},// level Q
        { 24,  3, 12,  8, 13} // level H
    },
    { //v12
        { 24,  2, 92,  2, 93},// level L
        { 22,  6, 36,  2, 37},// level M
        { 26,  4, 20,  6, 21},// level Q
        { 28,  7, 14,  4, 15} // level H
    },
    { //v13
        { 26,  4,107,  0,  0},// level L
        { 22,  8, 37,  1, 38},// level M
        { 24,  8, 20,  4, 21},// level Q
        { 22, 12, 11,  4, 12} // level H
    },
    { //v14
        { 30,  3,115,  1,116},// level L
        { 24,  4, 40,  5, 41},// level M
        { 20, 11, 16,  5, 17},// level Q
        { 24, 11, 12,  5, 13} // level H
    },
    { //v15
        { 22,  5, 87,  1, 88},// level L
        { 24,  5, 41,  5, 42},// level M
        { 30,  5, 24,  7, 25},// level Q
        { 24, 11, 12,  7, 13} // level H
    },
    { //v16
        { 24,  5, 98,  1, 99},// level L
        { 28,  7, 45,  3, 46},// level M
        { 24, 15, 19,  2, 20},// level Q
        { 30,  3, 15, 13, 16} // level H
    },
    { //v17
        { 28,  1,107,  5,108},// level L
        { 28, 10, 46,  1, 47},// level M
        { 28,  1, 22, 15, 23},// level Q
        { 28,  2, 14, 17, 15} // level H
    },
    { //v18
        { 30,  5,120,  1,121},// level L
        { 26,  9, 43,  4, 44},// level M
        { 28, 17, 22,  1, 23},// level Q
        { 28,  2, 14, 19, 15} // level H
    },
    { //v19
        { 28,  3,113,  4,114},// level L
        { 26,  3, 44, 11, 45},// level M
        { 26, 17, 21,  4, 22},// level Q
        { 26,  9, 13, 16, 14} // level H
    },
    { //v20
        { 28,  3,107,  5,108},// level L
        { 26,  3, 41, 13, 42},// level M
        { 30, 15, 24,  5, 25},// level Q
        { 28, 15, 15, 10, 16} // level H
    },
    { //v21
        { 28,  4,116,  4,117},// level L
        { 26, 17, 42,  0,  0},// level M
        { 28, 17, 22,  6, 23},// level Q
        { 30, 19, 16,  6, 17} // level H
    },
    { //v22
        { 28,  2,111,  7,112},// level L
        { 28, 17, 46,  0,  0},// level M
        { 30,  7, 24, 16, 25},// level Q
        { 24, 34, 13,  0,  0} // level H
    },
    { //v23
        { 30,  4,121,  5,122},// level L
        { 28,  4, 47, 14, 48},// level M
        { 30, 11, 24, 14, 25},// level Q
        { 30, 16, 15, 14, 16} // level H
    },
    { //v24
        { 30,  6,117,  4,118},// level L
        { 28,  6, 45, 14, 46},// level M
        { 30, 11, 24, 16, 25},// level Q
        { 30, 30, 16,  2, 17} // level H
    },
    { //v25
        { 26,  8,106,  4,107},// level L
        { 28,  8, 47, 13, 48},// level M
        { 30,  7, 24, 22, 25},// level Q
        { 30, 22, 15, 13, 16} // level H
    },
    { //v26
        { 28, 10,114,  2,115},// level L
        { 28, 19, 46,  4, 47},// level M
        { 28, 28, 22,  6, 23},// level Q
        { 30, 33, 16,  4, 17} // level H
    },
    { //v27
        { 30,  8,122,  4,123},// level L
        { 28, 22, 45,  3, 46},// level M
        { 30,  8, 23, 26, 24},// level Q
        { 30, 12, 15, 28, 16} // level H
    },
    { //v28
        { 30,  3,117, 10,118},// level L
        { 28,  3, 45, 23, 46},// level M
        { 30,  4, 24, 31, 25},// level Q
        { 30, 11, 15, 31, 16} // level H
    },
    { //v29
        { 30,  7,116,  7,117},// level L
        { 28, 21, 45,  7, 46},// level M
        { 30,  1, 23, 37, 24},// level Q
        { 30, 19, 15, 26, 16} // level H
    },
    { //v30
        { 30,  5,115, 10,116},// level L
        { 28, 19, 47, 10, 48},// level M
        { 30, 15, 24, 25, 25},// level Q
        { 30, 23, 15, 25, 16} // level H
    },
    { //v31
        { 30, 13,115,  3,116},// level L
        { 28,  2, 46, 29, 47},// level M
        { 30, 42, 24,  1, 25},// level Q
        { 30, 23, 15, 28, 16} // level H
    },
    { //v32
        { 30, 17,115,  0,  0},// level L
        { 28, 10, 46, 23, 47},// level M
        { 30, 10, 24, 35, 25},// level Q
        { 30, 19, 15, 35, 16} // level H
    },
    { //v33
        { 30, 17,115,  1,116},// level L
        { 28, 14, 46, 21, 47},// level M
        { 30, 29, 24, 19, 25},// level Q
        { 30, 11, 15, 46, 16} // level H
    },
    { //v34
        { 30, 13,115,  6,116},// level L
        { 28, 14, 46, 23, 47},// level M
        { 30, 44, 24,  7, 25},// level Q
        { 30, 59, 16,  1, 17} // level H
    },
    { //v35
        { 30, 12,121,  7,122},// level L
        { 28, 12, 47, 26, 48},// level M
        { 30, 39, 24, 14, 25},// level Q
        { 30, 22, 15, 41, 16} // level H
    },
    { //v36
        { 30,  6,121, 14,122},// level L
        { 28,  6, 47, 34, 48},// level M
        { 30, 46, 24, 10, 25},// level Q
        { 30,  2, 15, 64, 16} // level H
    },
    { //v37
        { 30, 17,122,  4,123},// level L
        { 28, 29, 46, 14, 47},// level M
        { 30, 49, 24, 10, 25},// level Q
        { 30, 24, 15, 46, 16} // level H
    },
    { //v38
        { 30,  4,122, 18,123},// level L
        { 28, 13, 46, 32, 47},// level M
        { 30, 48, 24, 14, 25},// level Q
        { 30, 42, 15, 32, 16} // level H
    },
    { //v39
        { 30, 20,117,  4,118},// level L
        { 28, 40, 47,  7, 48},// level M
        { 30, 43, 24, 22, 25},// level Q
        { 30, 10, 15, 67, 16} // level H
    },
    { //v40
        { 30, 19,118,  6,119},// level L
        { 28, 18, 47, 31, 48},// level M
        { 30, 34, 24, 34, 25},// level Q
        { 30, 20, 15, 61, 16} // level H
    } 
};
static int qr_remainder_bits [] = {0, 7, 7, 7, 7, 7, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, };
static int qr_alignment_location [][7] =
{
	{ },
    { 6,18,},
    { 6,22,},
    { 6,26,},
    { 6,30,},
    { 6,34,},
    { 6,22,38,},
    { 6,24,42,},
    { 6,26,46,},
    { 6,28,50,},
    { 6,30,54,},
    { 6,32,58,},
    { 6,34,62,},
    { 6,26,46,66,},
    { 6,26,48,70,},
    { 6,26,50,74,},
    { 6,30,54,78,},
    { 6,30,56,82,},
    { 6,30,58,86,},
    { 6,34,62,90,},
    { 6,28,50,72,94,},
    { 6,26,50,74,98,},
    { 6,30,54,78,102,},
    { 6,28,54,80,106,},
    { 6,32,58,84,110,},
    { 6,30,58,86,114,},
    { 6,34,62,90,118,},
    { 6,26,50,74,98,122,},
    { 6,30,54,78,102,126,},
    { 6,26,52,78,104,130,},
    { 6,30,56,82,108,134,},
    { 6,34,60,86,112,138,},
    { 6,30,58,86,114,142,},
    { 6,34,62,90,118,146,},
    { 6,30,54,78,102,126,150,},
    { 6,24,50,76,102,128,154,},
    { 6,28,54,80,106,132,158,},
    { 6,32,58,84,110,136,162,},
    { 6,26,54,82,110,138,166,},
    { 6,30,58,86,114,142,170,},
};

static const char * format [] = {
        "111011111000100",
        "111001011110011",
        "111110110101010",
        "111100010011101",
        "110011000101111",
        "110001100011000",
        "110110001000001",
        "110100101110110",

        "101010000010010",
        "101000100100101",
        "101111001111100",
        "101101101001011",
        "100010111111001",
        "100000011001110",
        "100111110010111",
        "100101010100000",

        "011010101011111",
        "011000001101000",
        "011111100110001",
        "011101000000110",
        "010010010110100",
        "010000110000011",
        "010111011011010",
        "010101111101101",

        "001011010001001",
        "001001110111110",
        "001110011100111",
        "001100111010000",
        "000011101100010",
        "000001001010101",
        "000110100001100",
        "000100000111011",

    "000111110010010100", // v7
    "001000010110111100", // v8
    "001001101010011001", // v9
    "001010010011010011", // v10
    "001011101111110110", // v11
    "001100011101100010", // v12
    "001101100001000111", // v13
    "001110011000001101", // v14
    "001111100100101000", // v15
    "010000101101111000", // v16
    "010001010001011101", // v17
    "010010101000010111", // v18
    "010011010100110010", // v19
    "010100100110100110", // v20
    "010101011010000011", // v21
    "010110100011001001", // v22
    "010111011111101100", // v23
    "011000111011000100", // v24
    "011001000111100001", // v25
    "011010111110101011", // v26
    "011011000010001110", // v27
    "011100110000011010", // v28
    "011101001100111111", // v29
    "011110110101110101", // v30
    "011111001001010000", // v31
    "100000100111010101", // v32
    "100001011011110000", // v33
    "100010100010111010", // v34
    "100011011110011111", // v35
    "100100101100001011", // v36
    "100101010000101110", // v37
    "100110101001100100", // v38
    "100111010101000001", // v39
    "101000110001101001", // v40
};
