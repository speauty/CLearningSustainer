//
// Created by speauty on 2021/2/18.
//

#include <stdlib.h>
#include "queue.h"
#include "list.h"

/**
 * 从队列尾部插入元素
 * @param queue
 * @param data
 * @return
 */
int queue_enqueue(Queue *queue, const void *data)
{
    return list_ins_next(queue, list_tail(queue), data);
}

/**
 * 从队列头部移除元素
 * @param queue
 * @param data
 * @return
 */
int queue_dequeue(Queue *queue, void **data)
{
    return list_rem_next(queue, NULL, data);
}
