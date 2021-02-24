//
// Created by speauty on 2021/2/24.
//

#ifndef DEMO_HEAP_H
#define DEMO_HEAP_H

/** 堆 */

typedef struct __Heap
{
    int size;
    int (*compare) (const void *key1, const void *key2);
    void (*destroy) (void *data);
    void **tree;
} Heap;

#define heap_size(heap) ((heap)->size)

void heap_init(Heap *heap, int (*compare) (const void *key1, const void *key2), void (*destroy) (void *data));
void heap_destroy(Heap *heap);
int heap_insert(Heap *heap, const void *data);
/** 释放堆顶部的节点 */
int heap_extract(Heap *heap, void **data);

#endif //DEMO_HEAP_H
