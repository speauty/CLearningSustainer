//
// Created by speauty on 2021/2/24.
//


#include <stdlib.h>
#include <string.h>
#include "heap.h"

/** 指定位置节点的父节点 */
#define heap_parent(position) ((int)(((position)-1)/2))
/** 指定位置节点的左子节点 */
#define heap_left(position) (((position)*2)+1)
/** 指定位置节点的右子节点 */
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
    /** 插入和上级的节点位置 */
    int insertPosition, parentPosition;
    if (NULL == (temp = (void **)realloc(heap->tree, (heap_size(heap)+1)* sizeof(void *)))) {
        return -1;
    } else {
        heap->tree = temp;
    }
    /** 插入尾节点 */
    heap->tree[heap_size(heap)] = (void *)data;
    insertPosition = heap_size(heap);
    parentPosition = heap_parent(insertPosition);
    while (insertPosition > 0 && heap->compare(heap->tree[parentPosition], heap->tree[insertPosition]) < 0) {
        /** 父节点数据小于当前插入节点, 需要做一次交换 */
        temp = heap->tree[parentPosition];
        heap->tree[parentPosition] = heap->tree[insertPosition];
        heap->tree[insertPosition] = temp;
        /** 向上处理 */
        insertPosition = parentPosition;
        parentPosition = heap_parent(insertPosition);
    }
    heap->size++;
    return 0;
}

int heap_extract(Heap *heap, void **data)
{
    void *save, *temp;
    /** 插入/左子/右子/最大子节点位置 */
    int insertPosition, leftPosition, rightPosition, maxPosition;

    if (heap_size(heap) == 0) return -1;
    /** 获取堆头数据 */
    *data = heap->tree[0];
    /** 需要保存的 */
    save = heap->tree[heap_size(heap)-1];
    if ((heap_size(heap)-1) > 0) {
        /** 堆中还有其他元素 */
        if (NULL == (temp = (void **)realloc(heap->tree, (heap_size(heap)-1)*sizeof(void *)))) {
            return -1;
        } else {
            heap->tree = temp;
        }
        heap->size--;
    } else {
        /** 这是在释放堆中最后一个元素了 */
        free(heap->tree);
        heap->tree = NULL;
        heap->size = 0;
        return 0;
    }
    heap->tree[0] = save;

    insertPosition = 0;
    while (1) {
        leftPosition = heap_left(insertPosition);
        rightPosition = heap_right(insertPosition);
        if (leftPosition < heap_size(heap) && heap->compare(heap->tree[leftPosition], heap->tree[insertPosition]) > 0) {
            maxPosition = leftPosition;
        } else {
            maxPosition = insertPosition;
        }

        if (rightPosition < heap_size(heap) && heap->compare(heap->tree[rightPosition], heap->tree[maxPosition]) > 0) {
            maxPosition = rightPosition;
        }

        if (maxPosition == insertPosition) {
            break;
        } else {
            temp = heap->tree[maxPosition];
            heap->tree[maxPosition] = heap->tree[insertPosition];
            heap->tree[insertPosition] = temp;
            /** 向下处理 */
            insertPosition = maxPosition;
        }
    }
    return 0;
}























