//
// Created by speauty on 2021/3/2.
//

#ifndef DEMO_SEARCH_H
#define DEMO_SEARCH_H

/**
 * 二分查找
 * 不断地将有序数据集进行对半分割, 并查检每个分区的中间元素
 * @param sorted 有序数据集 连续的存储空间
 * @param target 要查找的数据
 * @param size 元素的数量
 * @param eSize 元素的大小
 * @param compare
 * @return
 */
int biSearch(void *sorted, void *target, int size, int eSize, int (*compare) (const void *key1, const void *key2));

#endif //DEMO_SEARCH_H
