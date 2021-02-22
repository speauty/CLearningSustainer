//
// Created by speauty on 2021/2/22.
//

#ifndef DEMO_OHASHTABLE_H
#define DEMO_OHASHTABLE_H

/** 开地址哈希表(open-addressed hashtable) */
typedef struct __OHashTable
{
    /** positions 槽位的个数 */
    int positions;
    /**
    * 指针vacated将被初始化来指向一个特殊的地址空间，来指明这个特殊地址上曾经删除一个元素
    * 一个开地址哈希表的空槽通常包含一个空指针NULL
    * 当删除一个元素时，不能将删除元素的数据指向NULL
    * 由于当查找接下来的元素时，NULL表明此槽位是空的，随之探查过程将停止
    * 这样一个或多个元素可能被插入之前删除过元素的槽中，但实际上槽中的元素还存在
    * 当删除元素时，将哈希表中数据的指针指向vacated成员
    * vacated的地址就像一个哨兵，用于指明新元素将要插入的槽位
    */
    void *vacated;
    /** h1和h2用来指定用户定义的辅助哈希函数来完成双散列过程 */
    int (*h1) (const void *key);
    int (*h2) (const void *key);
    /** 判断两个键是否匹配的函数 */
    int (*match) (const void *key1, const void *key2);
    /** 释放动态分配的内存空间 */
    void (*destroy) (void *data);
    int size;
    void **table;
} OHashTable;

#define oht_size(oHashTable) ((oHashTable)->size);

int oht_init(
        OHashTable *oHashTable, int positions,
    int (*h1) (const void *key), int (*h2) (const void *key),
    int (*match) (const void *key1, const void *key2),
    void (*destroy) (void *data)
);
void oht_destroy(OHashTable *oHashTable);
int oht_insert(OHashTable *oHashTable, const void *data);
int oht_remove(OHashTable *oHashTable, void **data);
int oht_lookup(OHashTable *oHashTable, void **data);

#endif //DEMO_OHASHTABLE_H
