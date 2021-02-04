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
typedef struct __List
{
    /** 链表元素数量 */
    int size;
    int (*match) (const void *keyFirst, const void *keySecond);
    /** 释放元素数据存储的函数 */
    void (*destroy) (void *data);
    /** 链表头指针和尾指针, 并且在单向链表中, tail的next永远为NULL */
    ListElement *head;
    ListElement *tail;
} List;

/** 开放接口 */
void listInit(List *list, void(*destroy) (void *data));
void listDestroy(List *list);
int listInsNext(List *list, ListElement *element, const void *data);
int listRemNext(List *list, ListElement *element, void **data);
#define listSize(list) ((list)->size)
#define listHead(list) ((list)->head)
#define listTail(list) ((list)->tail)
#define listIsHead(list, element) ((element)==(list)->head?1:0)
#define listIsTail(list, element) ((element)==(list)->tail?1:0)
#define listData(element) ((element)->data)
#define listNext(element) ((element)->next)

#endif //DEMO_LIST_H
