//
// Created by speauty on 2021/2/4.
//

#ifndef DEMO_LIST_H
#define DEMO_LIST_H
#include <stdlib.h>

/** 链表元素结构体定义 */
typedef struct __ListElement
{
    /** 当前元素包含的数据指针, 指向具体的数据块 */
    void *data;
    /** 当前元素的下一个元素指针, 由于没有上游指针, 只能单向传递 */
    struct __ListElement *next;
} ListElement;
/** 链表结构定义 */
typedef struct __list
{
    /** 链表元素数量 */
    int size;
    int (*match) (const void *keyFirst, const void *keySecond);
    /** 释放动态分配空间(也就是这里分配的数据空间)的函数 */
    void (*destroy) (void *data);
    /** 链表头指针和尾指针, 并且在单向链表中, tail的next永远为NULL */
    ListElement *head;
    ListElement *tail;
} List;

/** 开放接口 */
void list_init(List *list, void(*destroy) (void *data));
void list_destroy(List *list);
int list_ins_next(List *list, ListElement *element, const void *data);
int list_rem_next(List *list, ListElement *element, void **data);
#define list_size(list) ((list)->size)
#define list_head(list) ((list)->head)
#define list_tail(list) ((list)->tail)
#define list_is_head(list, element) ((element)==(list)->head?1:0)
#define list_is_tail(list, element) ((element)==(list)->tail?1:0)
#define list_data(element) ((element)->data)
#define list_next(element) ((element)->next)

#endif //DEMO_LIST_H
