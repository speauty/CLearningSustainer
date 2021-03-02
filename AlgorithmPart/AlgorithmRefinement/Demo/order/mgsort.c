//
// Created by speauty on 2021/3/2.
//
/** 归并排序实现 */
#include <stdlib.h>
#include <string.h>
#include "sort.h"

/**
 * 将两个有序集合合并成一个有序集
 * 将data中i到j之间的数据集与j+1到k之间的数据集合并成一个i到k的有序数据集
 * @param data
 * @param eSize
 * @param i
 * @param j
 * @param k
 * @param compare
 * @return
 */
static int merge(void *data, int eSize, int i, int j, int k, int (*compare) (const void *key1, const void *key2))
{
    /**
     * mPtr 存放新的有序数据集
     */
    char *dataPtr = data, *mPtr;
    int iPos, jPos, mPos;
    /** i<j<k 初始化 */
    iPos = i;
    jPos = j+1;
    mPos = 0;
    /** 分配存储空间 */
    if (NULL == (mPtr = (char *)malloc(eSize*((k-i)+1)))) return -1;
    /** 两端到头 */
    while (iPos <= j || jPos <= k) {
        if (iPos > j) {
            /** 如果左段偏移大于j, 说明左段偏移到头, 右段直接插入处理 */
            while (jPos <= k) {
                /** 将data的jPos数据拷贝到新集合的mPos位置处 */
                memcpy(&mPtr[mPos*eSize], &dataPtr[jPos*eSize], eSize);
                jPos++;
                mPos++;
            }
            continue;
        } else if (jPos > k) {
            /** 如果右段偏移大于k, 说明右段偏移到头, 左段直接插入处理 */
            while (iPos <= j) {
                /** 将data的iPos数据拷贝到新集合的mPos位置处 */
                memcpy(&mPtr[mPos*eSize], &dataPtr[iPos*eSize], eSize);
                iPos++;
                mPos++;
            }
            continue;
        }

        /** 比较两段当前值大小, 哪边小就把对应值插入有序数据集, 并做好偏移记录 */
        if (compare(&dataPtr[iPos*eSize], &dataPtr[jPos*eSize]) < 0) {
            memcpy(&mPtr[mPos*eSize], &dataPtr[iPos*eSize], eSize);
            iPos++;
            mPos++;
        } else {
            memcpy(&mPtr[mPos*eSize], &dataPtr[jPos*eSize], eSize);
            jPos++;
            mPos++;
        }
    }
    /** 全量拷贝 */
    memcpy(&dataPtr[i*eSize], mPtr, eSize*((k-i)+1));
    free(mPtr);
    return 0;
}

int mgSort(void *data, int size, int eSize, int i, int k, int (*compare) (const void *key1, const void *key2))
{
    int j;
    if (i < k) {
        j = (int) ((i+k-1)/2);
        if (mgSort(data, size, eSize, i, j, compare) < 0) return -1;
        if (mgSort(data, size, eSize, j+1, k, compare) < 0) return -1;
        if (merge(data, eSize, i, j, k, compare) < 0) return -1;
    }
    return 0;
}