//
// Created by speauty on 2021/2/24.
//


#include <stdlib.h>
#include <string.h>
#include "heap.h"

#define heap_parent(position) ((int)(((position)-1)/2))
#define heap_left(position) (((position)*2)+1)
#define heap_right(position) (((position)*2)+2)

void heap_init(Heap *heap, int (*compare) (const void *key1, const void *key2), void (*destroy) (void *data))
{
    heap->size = 0;
    heap->compare = compare;
    heap->destroy = destroy;
    heap->tree = NULL;
}

void heap_destroy(Heap *heap)
{
    int i;
    if (NULL != heap->destroy) {
        for (i = 0; i < heap_size(heap); ++i) {
            heap->destroy(heap->tree[i]);
        }
    }
    free(heap->tree);
    memset(heap, 0, sizeof(Heap));
}

int heap_insert(Heap *heap, const void *data)
{
    void *temp;
    int insertPosition, popPosition;
    if (NULL == (temp = (void **)realloc(heap->tree, (heap_size(heap)+1)* sizeof(void *)))) {
        return -1;
    } else {
        heap->tree = temp;
    }
    /** 插入尾节点 */
    heap->tree[heap_size(heap)] = (void *)data;
    insertPosition = heap_size(heap);
    popPosition = heap_parent(insertPosition);
}























