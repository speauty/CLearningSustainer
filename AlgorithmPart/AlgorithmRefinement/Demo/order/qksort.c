//
// Created by speauty on 2021/3/2.
//
#include <stdlib.h>
#include <string.h>
#include "sort.h"

static int compare_int(const void *int1, const void *int2)
{
    if (*(const int *) int1 > *(const int *) int2) {
        return 1;
    } else if (*(const int *) int1 < *(const int *) int2) {
        return -1;
    } else {
        return 0;
    }
}

/**
 * 分区
 * @param data
 * @param eSize
 * @param i
 * @param k
 * @param compare
 * @return
 */
static int partition(void *data, int eSize, int i, int k, int (*compare) (const void *key1, const void *key2))
{
    char *dataPtr = data;
    void *partitionVal, *temp;
    /** 三个位置的整型数组, 中值获取分区值(中位数法) */
    int r[3];

    /** 分配存储空间 */
    if (NULL == (partitionVal = malloc(eSize))) return -1;
    if (NULL == (temp = malloc(eSize))) {
        free(partitionVal);
        return -1;
    }

    /** 随机生成三个整型数据 */
    r[0] = (rand()%(k-i+1))+i;
    r[1] = (rand()%(k-i+1))+i;
    r[2] = (rand()%(k-i+1))+i;
    /** 对数组进行排序 */
    isSort(r, 3, sizeof(int), compare_int);
    /** 获取中值r[1]指向的分区点 */
    memcpy(partitionVal, &dataPtr[r[1]*eSize], eSize);
    /** 这里的自增自减运算作用不太清楚 */
    i--;
    k++;

    /** 死循环 */
    while (1) {
        /** 左边分区: 向左偏移运算直到找到比分区点小的点或越界 */
        do {
            k--;
        } while (compare(&dataPtr[k*eSize], partitionVal) > 0);
        /** 右边分区: 向右偏移运算直到找到比分区点大的点或越界 */
        do {
            i++;
        } while (compare(&dataPtr[i*eSize], partitionVal) < 0);

        /** 如果向右偏移量不小于向左偏移量, 就退出循环 */
        /** 一旦i和k重合, 那么所有处于左边的元素将小于等于它, 所有处于右边的值将大于等于它 */
        if (i >= k) {
            break;
        } else {
            /** 到这里的话, k值是小于i值的*/
            /** 否则交换i和k位置的值继续进入下一轮循环 */
            memcpy(temp, &dataPtr[i*eSize], eSize);
            memcpy(&dataPtr[i*eSize], &dataPtr[k*eSize], eSize);
            memcpy(&dataPtr[k*eSize], temp, eSize);
            /** 交换之后, k值是大于i值的, 分区值为中间点 */
        }
    }

    /** 释放存储空间 */
    free(partitionVal);
    free(temp);
    return k;
}

/** 快排 */
int qkSort(void *data, int size, int eSize, int i, int k, int (*compare) (const void *key1, const void *key2))
{
    int j;
    while (i < k) {
        /** 获取分区点 */
        if ((j = partition(data, eSize, i, k, compare)) < 0) return -1;
        /** 继续往下分 */
        if (qkSort(data, size, eSize, i, j, compare) < 0) return -1;
        i = j+1;
    }
    return 0;
}
