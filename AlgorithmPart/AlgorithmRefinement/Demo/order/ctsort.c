//
// Created by speauty on 2021/3/2.
//
/** 计数排序实现 */
#include <stdlib.h>
#include <string.h>
#include "sort.h"

/**
 * 计数排序
 * @param data
 * @param size
 * @param k
 * @return
 */
int ctSort(int *data, int size, int k)
{
    int *counts, *temp;
    int i, j;
    /** 初始化处理 */
    if (NULL == (counts = (int *)malloc(k*sizeof(int)))) return -1;
    if (NULL == (temp = (int *)malloc(size*sizeof(int)))) return -1;
    for (i = 0; i < k; i++) counts[i] = 0;
    /** 统计统计元素出现的次数, 如果值越大, 那么位置越靠后 */
    for (j = 0; j < size; j++) counts[data[j]] = counts[data[j]] + 1;
    /** 累加上次 */
    for (i = 0; i < k; ++i) counts[i] = counts[i] + counts[i-1];

    for (j = size-1; j >= 0; j--) {
        temp[counts[data[j]]-1] = data[j];
        counts[data[j]] = counts[data[j]]-1;
    }

    memcpy(data, temp, size* sizeof(int));
    free(counts);
    free(temp);
    return 0;
}