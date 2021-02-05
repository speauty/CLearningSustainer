//
// Created by speauty on 2021/2/5.
//

#include "dlist.h"
#include <stdlib.h>
#include <string.h>

void dlist_init(DList *list, void(*destroy) (void *data))
{
    list->size = 0;
    list->destroy = destroy;
    list->head = NULL;
    list->tail = NULL;
}

void dlist_destroy(DList *list)
{
    void *data;
    while(dlist_size(list) > 0) {
        if (0 == dlist_remove(list, dlist_tail(list), (void **)&data) && NULL != list->destroy) {
            list->destroy(data);
        }
    }
    memset(list, 0, sizeof(DList));
}

int dlist_ins_next(DList *list, DListElement *element, const void *data)
{
    DListElement *newElement;
    if (NULL == (newElement = (DListElement *)malloc(sizeof(DListElement)))) return -1;
    newElement->data = (void *)data;
    if (0 == dlist_size(list)) {
        /** 没有元素的情况下, 头尾元素相同 */
        list->head = newElement;
        list->head->prev = NULL;
        list->head->next = NULL;
        list->tail = newElement;
    } else {
        if (NULL == element) return -1;
        newElement->next = element->next;
        newElement->prev = element;
        if (NULL == element->next) {
            /** 如果element为尾元素的话, 需要把新元素设置为尾元素 */
            list->tail = newElement;
        } else {
            element->next->prev = newElement;
        }
        element->next = newElement;
    }
    list->size++;
    return 0;
}

int dlist_ins_prev(DList *list, DListElement *element, const void *data)
{
    DListElement *newElement;
    if (NULL == (newElement = (DListElement *)malloc(sizeof(DListElement)))) return -1;
    newElement->data = (void *)data;

    if (0 == dlist_size(list)) {
        list->head = newElement;
        list->head->prev = NULL;
        list->head->next = NULL;
        list->tail = newElement;
    } else {
        if (NULL == element) return -1;
        newElement->next = element;
        newElement->prev = element->prev;
        if (NULL == element->prev) {
            list->head = newElement;
        } else {
            element->prev->next = newElement;
        }
        element->prev = newElement;
    }
    list->size++;
    return 0;
}

int dlist_remove(DList *list, DListElement *element, void **data)
{
    if (NULL == element || 0 == dlist_size(list)) return -1;
    *data = element->data;
    if (element == list->head) {
        /** 如果移除元素是头元素 */
        list->head = element->next;
        if (NULL == list->head) {
            /** 如果头元素为空, 说明已经没有元素了, 需要把尾元素设置为NULL */
            list->tail = NULL;
        } else {
            /** 把element元素的下游元素的上游指向设置为NULL */
            element->next->prev = NULL;
        }
    } else {
        element->prev->next = element->next;
        if (NULL == element->next) {
            list->tail = element->prev;
        } else {
            element->next->prev = element->prev;
        }
    }
    free(element);
    list->size--;
    return 0;
}
