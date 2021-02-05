//
// Created by speauty on 2021/2/5.
//

#ifndef DEMO_CLIST_H
#define DEMO_CLIST_H
#include <stdlib.h>

/** 循环链表元素结构定义 */
typedef struct __CListElement
{
    void *data;
    struct __CListElement *next;
} CListElement;
/** 循环链表结构定义 */
typedef struct __CList
{
    int size;
    int (*match) (const void *keyFirst, const void *keySecond);
    void (*destroy) (void *data);
    CListElement *head;
} CList;

void clist_init(CList *list, void (*destroy) (void *data));
void clist_destroy(CList *list);
int clist_ins_next(CList *list, CListElement *element, const void *data);
int clist_rem_next(CList *list, CListElement *element, void **data);
#define clist_size(list) ((list)->size)
#define clist_head(list) ((list)->head)
#define clist_data(element) ((element)->data)
#define clist_next(element) ((element)->next)

#endif //DEMO_CLIST_H
