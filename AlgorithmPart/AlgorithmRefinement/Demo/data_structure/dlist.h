//
// Created by speauty on 2021/2/5.
//

#ifndef DEMO_DLIST_H
#define DEMO_DLIST_H

/** 双向链表元素结构定义 */
typedef struct __DListElement
{
    void * data;
    /** 上游指针 */
    struct __DListElement * prev;
    /** 下游指针 */
    struct __DListElement * next;
} DListElement;
/** 双向链表结构定义 */
typedef struct __DList
{
    int size;
    int (*match) (const void * keyFirst, const void * keySecond);
    /** 释放动态分配的空间 */
    void (*destroy) (void *data);
    /** 头指针 */
    DListElement *head;
    /** 尾指针 */
    DListElement *tail;
} DList;

void dlist_init(DList *list, void (*destroy) (void *data));
void dlist_destroy(DList *list);
int dlist_ins_next(DList *list, DListElement *element, const void *data);
int dlist_ins_prev(DList *list, DListElement *element, const void *data);
int dlist_remove(DList *list, DListElement *element, void **data);
#define dlist_size(list) ((list)->size)
#define dlist_head(list) ((list)->head)
#define dlist_tail(list) ((list)->tail)
#define dlist_is_head(element) (NULL==(element)->prev?1:0)
#define dlist_is_tail(element) (NULL==(element)->next?1:0)
#define dlist_data(element) ((element)->data)
#define dlist_next(element) ((element)->next)
#define dlist_prev(element) ((element)->prev)

#endif //DEMO_DLIST_H
