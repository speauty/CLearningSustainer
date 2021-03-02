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

/**
 * 快速排序(一种分治排序算法)
 * 不断地将无序元素集递归分割，直到所有的分区都只包含单个元素
 * 分: 设定一个分割值并将数据分为两部分
 * 治: 分别在两部分用递归的方式继续使用快速排序法
 * 合: 对分割部分排序直至完成
 * @param data 需要进行排序的数组
 * @param size 元素的数量
 * @param eSize 元素的大小, 方便偏移运算
 * @param i 前分割点? 定义当前进行排序的两个部分, 0
 * @param k 后分割点? size-1
 * @param compare 自定义元素比较方法
 * @return
 */
int qkSort(void *data, int size, int eSize, int i, int k, int (*compare) (const void *key1, const void *key2));

/**
 * 归并排序(一种分治排序算法)
 * 将一个无序元素集分割成许多个只包含一个元素的集, 然后不断地将这些小集合并, 直到一个新的大有序数据集生成
 * 分: 将数据集等分为两半
 * 治: 分别在两个部分用递归的方式继续使用归并排序法
 * 合: 将分开的两个部分合并成一个有序的数据集
 * @param data 需要进行排序的数组
 * @param size 元素的数量
 * @param eSize 元素的大小, 方便偏移运算
 * @param i 前分割点? 定义当前进行排序的两个部分, 0
 * @param k 后分割点? size-1
 * @param compare 自定义元素比较方法
 * @return
 */
int mgSort(void *data, int size, int eSize, int i, int k, int (*compare) (const void *key1, const void *key2));








































#endif //DEMO_SORT_H
