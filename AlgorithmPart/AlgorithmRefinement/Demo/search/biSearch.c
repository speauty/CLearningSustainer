//
// Created by speauty on 2021/3/2.
//
/** 二分查找实现 */
#include <stdlib.h>
#include <string.h>
#include "search.h"

int biSearch(void *sorted, void *target, int size, int eSize, int (*compare) (const void *key1, const void *key2))
{
    int left, middle, right;
    left = 0;
    right = size-1;

    while (left <= right) {
        middle = (left+right)/2;
        switch (compare(((char *)sorted+(eSize*middle)), target)) {
            case -1:
                /** 向右找 */
                left = middle+1;break;
            case 1:
                /** 向左找 */
                right = middle-1;break;
            case 0:
                return middle;
        }
    }

    return -1;
}