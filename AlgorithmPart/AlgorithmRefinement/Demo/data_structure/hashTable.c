//
// Created by speauty on 2021/2/19.
//

#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "hashTable.h"

/**
 * 哈希表初始化
 * @param hashTable 哈希表
 * @param buckets 桶的数量
 * @param h 哈希编码函数
 * @param match 匹配函数
 * @param destroy 释放函数
 * @return
 */
int ht_init(
    HashTable *hashTable, int buckets, int (*h) (const void *key),
    int (*match) (const void *key1, const void *key2),
    void (*destroy) (void *data)
) {
    int i;
    /** 分配存储空间 */
    if (NULL == hashTable->table = (List *)malloc(buckets *sizeof(List))) return -1;
    /** 初始化哈希表桶数量 */
    hashTable->buckets = buckets;
    /** 初始化哈希表中的桶(普通链表) */
    for (i = 0; i < hashTable->buckets; ++i) list_init(&hashTable->table[i], destroy);
    hashTable->h = h;
    hashTable->match = match;
    hashTable->destroy = destroy;
    /** 初始化哈希表元素数量 */
    hashTable->size = 0;
    return 0;
}

/**
 * 销毁哈希表
 * @param hashTable 指定哈希表
 */
void ht_destroy(HashTable *hashTable)
{
    int i;
    /** 依次销毁哈希表的桶 */
    for (i = 0; i < hashTable->size; ++i) list_destroy(&hashTable->table[i]);
    /** 释放哈希表的table空间 */
    free(hashTable->table);
    /** "初始化"哈希表 */
    memset(hashTable, 0, sizeof(hashTable));
}

/**
 * 向指定哈希表插入新元素
 * 以桶头元素插入
 * @param hashTable
 * @param data
 * @return
 */
int ht_insert(HashTable *hashTable, const void *data)
{
    void *temp;
    int bucket,result;
    temp = (void *)data;
    /** 如果数据在哈希表中存在, 直接返回 */
    if (0 == ht_lookup(hashTable, &temp)) return 1;
    /** 生成哈希编码[确定桶的位置] */
    bucket = hashTable->h(data)%hashTable->buckets;
    /** 从头插入 */
    if (0 == (result = list_ins_next(&hashTable->table[bucket], NULL, data))) {
        hashTable->size++;
    }
    return result;
}

/**
 * 从指定哈希表中移除元素
 * @param hashTable
 * @param data
 * @return
 */
int ht_remove(HashTable *hashTable, void **data)
{
    ListElement *element, *prev;
    int bucket;
    bucket = hashTable->h(*data)%hashTable->buckets;
    prev = NULL;
    for (element = list_head(&hashTable->table[bucket]); element != NULL; element = list_next(element)) {
        if (hashTable->match(*data, list_data(element))) {
            if (0 == list_rem_next(&hashTable->table[bucket], prev, data)) {
                hashTable->size--;
                return 0;
            } else {
                return -1;
            }
        }
        prev = element;
    }
    /** 未找到相应元素 */
    return -1;
}

/**
 * 在指定哈希表中查找元素
 * @param hashTable
 * @param data
 * @return
 */
int ht_lookup(const HashTable *hashTable, void **data)
{
    ListElement *element;
    int bucket;
    bucket = hashTable->h(*data)%hashTable->buckets;
    for (element = list_head(&hashTable->table[bucket]); element != NULL; element = list_next(element)) {
        if (hashTable->match(*data, list_data(element))) {
            *data = list_data(element);
            return 0;
        }
    }
    return -1;
}
