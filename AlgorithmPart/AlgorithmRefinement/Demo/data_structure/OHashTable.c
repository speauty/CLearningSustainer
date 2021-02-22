//
// Created by speauty on 2021/2/22.
//

#include <stdlib.h>
#include <string.h>
#include "OHashTable.h"

static char vacated;

/**
 * 开地址哈希表初始化
 * @param oHashTable
 * @param positions
 * @param h1
 * @param h2
 * @param match
 * @param destroy
 * @return
 */
int oht_init(
        OHashTable *oHashTable, int positions,
    int (*h1) (const void *key), int (*h2) (const void *key),
    int (*match) (const void *key1, const void *key2),
    void (*destroy) (void *data)
)
{
    int i;
    if (NULL == (oHashTable->table = (void **)malloc(positions* sizeof(void *)))) return -1;
    oHashTable->positions = positions;
    for (i = 0; i < oHashTable->positions; ++i) oHashTable->table[i] = NULL;
    oHashTable->vacated = &vacated;
    oHashTable->h1 = h1;
    oHashTable->h2 = h2;
    oHashTable->match = match;
    oHashTable->destroy = destroy;
    oHashTable->size = 0;
    return 0;
}


/**
 * 销毁哈希表
 * @param oHashTable
 */
void oht_destroy(OHashTable *oHashTable)
{
    int i;
    if (NULL != oHashTable->destroy) {
        for (i = 0; i < oHashTable->positions; ++i) {
            if (NULL != oHashTable->table[i] && oHashTable->vacated != oHashTable->table[i]) {
                oHashTable->destroy(oHashTable->table[i]);
            }
        }
    }
    free(oHashTable->table);
    memset(oHashTable, 0, sizeof(OHashTable));
}

/**
 * 插入元素
 */
int oht_insert(OHashTable *oHashTable, const void *data)
{
    void *temp;
    int position, i;
    if (oHashTable->size == oHashTable->positions) return -1;
    temp = (void *)data;
    if (0 == oht_lookup(oHashTable, &temp)) return 1;
    for (i = 0; i < oHashTable->positions; ++i) {
        position = (oHashTable->h1(data)+i*oHashTable->h2(data))%oHashTable->positions;
        if (NULL == oHashTable->table[position] || oHashTable->vacated == oHashTable->table[position]) {
            oHashTable->table[position] = (void *)data;
            oHashTable->size++;
            return 0;
        }
    }
    /** 应该是哈希散列函数有问题 */
    return -1;
}

/**
 * 移除指定元素
 * @param oHashTable
 * @param data
 * @return
 */
int oht_remove(OHashTable *oHashTable, void **data)
{
    int position,i;
    for (int i = 0; i < oHashTable->positions; ++i) {
        position = (oHashTable->h1(data)+i*oHashTable->h2(data))%oHashTable->positions;
        if (NULL == oHashTable->table[position]) {
            /** 未找到相应元素 */
            return -1;
        } else if (oHashTable->vacated == oHashTable->table[position]) {
            continue;
        } else if (oHashTable->match(oHashTable->table[position], *data)) {
            *data = oHashTable->table[position];
            /** 将对应槽置为vacated */
            oHashTable->table[position] = oHashTable->vacated;
            oHashTable->size--;
            return 0;
        }
    }
    /** 未找到相应元素 */
    return -1;
}

/**
 * 查找指定元素
 * @param oHashTable
 * @param data
 * @return
 */
int oht_lookup(OHashTable *oHashTable, void **data)
{
    int position, i;
    for (i = 0; i < oHashTable->positions; ++i) {
        position = (oHashTable->h1(data)+i*oHashTable->h2(data))%oHashTable->positions;
        if (NULL == oHashTable->table[position]) {
            /** 未找到相应元素 */
            return -1;
        } else if (oHashTable->match(oHashTable->table[position], *data)) {
            *data = oHashTable->table[position];
            return 0;
        }
    }
    return -1;
}