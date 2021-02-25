//
// Created by speauty on 2021/2/25.
//

#ifndef DEMO_PRIORITYQUEUE_H
#define DEMO_PRIORITYQUEUE_H

#include "heap.h"
/** 优先队列 */
typedef Heap PriorityQueue;

#define priority_queue_size heap_size
#define priority_queue_init heap_init
#define priority_queue_destroy heap_destroy
#define priority_queue_insert heap_insert
#define priority_queue_extract heap_extract
#define priority_queue_peek(priorityQueue) (NULL == (priorityQueue)->tree?NULL:(priorityQueue)->tree[0])

#endif //DEMO_PRIORITYQUEUE_H
