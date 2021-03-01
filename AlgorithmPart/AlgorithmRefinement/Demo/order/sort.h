//
// Created by speauty on 2021/3/1.
//

#ifndef DEMO_SORT_H
#define DEMO_SORT_H

/** 排序算法 */

/***
 * 插入排序
 * 插入排序每次从无序数据集中取出一个元素, 扫描已排好序的数据集, 并将它插入有序集合合适的位置上
 */
/**
 * 插入排序申明
 * @param data 需要排序的数组
 * @param size 元素个数
 * @param eSize 元素的大小
 * @param compare 用户自定义的比较函数
 * @return
 */
int isSort(void *data, int size, int eSize, int (*compare) (const void *key1, const void *key2));

#endif //DEMO_SORT_H
