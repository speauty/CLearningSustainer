//
// Created by speauty on 2021/2/5.
//

#include "clist.h"
#include <stdlib.h>
#include <string.h>

void clist_init(CList *list, void(*destroy) (void *data))
{
    list->size = 0;
    list->destroy = destroy;
    list->head = NULL;
}

void clist_destroy(CList *list)
{
    void *data;
    while (0 < clist_size(list)) {
        if (0 == clist_rem_next(list, list->head, (void **)&data) && NULL != list->destroy) {
            list->destroy(data);
        }
    }
    memset(list, 0, sizeof(CList));
}

int clist_ins_next(CList *list, CListElement *element, const void *data)
{
    CListElement *newElement;
    if (NULL == (newElement = (CListElement *)malloc(sizeof(CListElement)))) return -1;
    newElement->data = data;

    if (0 == clist_size(list)) {
        newElement->next = newElement;
        list->head = newElement;
    } else {
        newElement->next = element->next;
        list->head = newElement;
    }
    list->size++;
    return 0;
}

int clist_rem_next(CList *list, CListElement *element, void **data)
{
    CListElement *oldElement;
    if (0 == clist_size(list)) return -1;
    *data = element->next->data;
    if (element->next == element) {
        oldElement= element;
        list->head = NULL;
    } else {
        oldElement = element->next;
        element->next = element->next->next;
        if (oldElement == clist_head(list)) {
            list->head = oldElement->next;
        }
    }
    free(oldElement);
    list->size--;
    return 0;
}