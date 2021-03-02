//
// Created by speauty on 2021/3/1.
//
#include <stdlib.h>
#include <string.h>
#include "sort.h"

/**
 * 插入排序
 * @param data
 * @param size
 * @param eSize
 * @param compare
 * @return
 */
int isSort(void *data, int size, int eSize, int (*compare) (const void *key1, const void *key2))
{
    char *dataPtr = data;
    void *key;
    int i,j;
    if (NULL == (key = (char *)malloc(eSize))) return -1;
    /** 向后移 */
    for (j = 1; j < size; j++) {
        /** 将第j个元素拷贝到key, 不影响j */
        memcpy(key, &dataPtr[j*eSize], eSize);
        /** i从0开始 */
        i = j - 1;
        /** i大于0并且第i个元素大于第j个元素(就是当前key)*/
        /** 向前移(持续交换) */
        while (i >= 0 && compare(&dataPtr[i*eSize], key) > 0) {
            /** 将第i个元素的内容拷贝到第i+1个元素 */
            /** 第一轮的i+1就是j位置的元素, i+1位置的已经在key存储 */
            memcpy(&dataPtr[(i+1)*eSize], &dataPtr[i*eSize], eSize);
            i--;
        }
        /** 已经找到了合适的位置 */
        /** 将key拷贝到i+1位置 */
        memcpy(&dataPtr[(i+1)*eSize], key, eSize);
    }
    free(key);
    return 0;
}
