//
// Created by speauty on 2021/2/18.
//

#ifndef DEMO_QUEUE_H
#define DEMO_QUEUE_H

#include <stdlib.h>
#include "list.h"

typedef List Queue;

#define queue_init list_init
#define queue_destroy list_destroy
/** 获取队列头部元素 */
#define queue_peek(queue) ((queue)->head==NULL?NULL:(queue)->head->data)
#define queue_size list_size
int queue_enqueue(Queue *queue, const void *data);
int queue_dequeue(Queue *queue, void **data);


#endif //DEMO_QUEUE_H
