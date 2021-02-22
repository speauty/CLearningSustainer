//
// Created by speauty on 2021/2/19.
//

#ifndef DEMO_HASHTABLE_H
#define DEMO_HASHTABLE_H

#include <stdlib.h>
#include "list.h"

/** 定义链式哈希表 */
typedef struct __HashTable
{
    /** 当前哈希表分配桶的个数 */
    int buckets;
    int (*h) (const void *key);
    int (*match) (const void *key1, const void *key2);
    void (*destroy) (void *data);
    /** 现有元素数量 */
    int size;
    /** 存储桶的数组 */
    List *table;
} HashTable;

#define ht_size(hashTable) ((hashTable)->size)

int ht_init(
    HashTable *hashTable, int buckets, int (*h) (const void *key),
    int (*match) (const void *key1, const void *key2),
    void (*destroy) (void *data)
);
void ht_destroy(HashTable *hashTable);
int ht_insert(HashTable *hashTable, const void *data);
int ht_remove(HashTable *hashTable, void **data);
int ht_lookup(const HashTable *hashTable, void **data);

#endif //DEMO_HASHTABLE_H
