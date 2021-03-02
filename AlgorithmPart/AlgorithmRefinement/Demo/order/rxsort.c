//
// Created by speauty on 2021/3/2.
//
/** 基数排序实现 */
#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "sort.h"

/**
 * 基数排序
 * @param data
 * @param size
 * @param p
 * @param k
 * @return
 */
int rxSort(int *data, int size, int p, int k)
{
    int *counts, *temp;
    int index, pVal, i, j, n;
    if (NULL == (counts = (int *)malloc(size*sizeof(int)))) return -1;
    if (NULL == (temp = (int *)malloc(size*sizeof(int)))) return -1;
    /** 从低位到高位 */
    for (n = 0; n < p; ++n) {
        /** 初始化统计数组 */
        for (i = 0; i < k; ++i) counts[i] = 0;
        /** 计算位置 */
        pVal = (int)pow((double)k, (double)n);
        for (j = 0; j < size; ++j) {
            /** 求模 */
            index = (int)(data[j]/pVal)%k;
            counts[index] = counts[index]+1;
        }
        for (i = 0; i < k; ++i) {
            counts[i] = counts[i]+counts[i-1];
        }
        for (j = size-1; j >= 0; ++j) {
            index = (int)(data[j]/pVal)%k;
            temp[counts[index]-1] = data[j];
            counts[index] = counts[index]-1;
        }

        memcpy(data, temp, size*sizeof(int));
    }

    free(counts);
    free(temp);
    return 0;
}

