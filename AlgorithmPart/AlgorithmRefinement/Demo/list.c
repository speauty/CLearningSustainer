//
// Created by speauty on 2021/2/4.
//

#include <stdlib.h>
#include <string.h>
#include "list.h"

/**
 * 链表初始化
 * @param list 需要初始化的链表
 * @param destroy
 */
void listInit(List *list, void(*destroy)(void *data))
{
    list->size = 0;
    list->destroy = destroy;
    list->head = NULL;
    list->tail = NULL;
}

/**
 * 销毁链表
 * @param list
 */
void listDestroy(List *list)
{
    /** 一级空指针类型 */
    void *data;
    /** 循环移除在链表中的每一个元素 */
    while (listSize(list) > 0) {
        /**
         * &data 取到指向data(本身就是个指针类型)的指针(二级指针)
         */
        if (0 == listRemNext(list, NULL, (void **)&data) && NULL != list->destroy) {
            list->destroy(data);
        }
    }
    /** 清空链表所有操作 */
    memset(list, 0, sizeof(List));
}

/**
 * 插入指定元素的下一个元素到指定链表中
 * @param list 指定链表
 * @param element 指定元素, 如果指定元素为NULL并且链表长度为0, 就是插入头元素
 * @param data 元素数据对象
 * @return
 */
int listInsNext(List *list, ListElement *element, const void *data)
{
    ListElement *newElement;
    /** 分配空间 */
    if (NULL == (newElement = (ListElement *)malloc(sizeof(ListElement)))) return -1;
    /** 将data转换类型后挂到新元素对应节点上 */
    newElement->data = (void *)data;

    if (NULL == element) {
        /** 以头元素形式插入 */
        /** 如果链表长度为0, 主动将尾元素也设置为新元素 */
        if (0 == listSize(list)) list->tail = newElement;
        /** 此处将为新元素的next指向head, 以链接原来的头元素(如果存在的情况) */
        newElement->next = list->head;
        list->head = newElement;
    } else {
        /** 如果指定元素的next为空, 则为尾元素 */
        if (NULL == element->next) list->tail = newElement;
        /**
         * 将新元素茶插入到指定元素后面
         * 将新元素的next指向指定元素的next, 然后将指定元素的next指向新元素
         */
        newElement->next = element->next;
        element->next = newElement;
    }
    /** 链表长度+1 */
    list->size++;
    return 0;
}

/**
 * 移除指定元素的下一个元素
 * @param list 指定链表
 * @param element 指定元素, 如果指定元素为NULL, 则移除头元素
 * @param data 二级空指针类型
 * @return
 */
int listRemNext(List *list, ListElement *element, void **data)
{
    ListElement *oldElement;
    /** 如果链表长度为0, 则直接返回-1, 表示当前链表为空, 不需处理 */
    if (0 == listSize(list)) return -1;

    if (NULL == element) {
        /**
         * data 二级指针类型
         * *data 取到一级指针空间
         * 将链表头元素数据指针存入一级指针空间, 即上层可获取对应数据指针
         */
        *data = list->head->data;
        oldElement = list->head;
        /** 重置头元素 */
        list->head = list->head->next;
        /** 如果只有一个元素时, 需要将尾元素置为NULL */
        if (1 == listSize(list)) list->tail = NULL;
    } else {
        /** 不能移除尾元素 */
        if (NULL == element->next) return -1;
        *data = element->next->data;
        /** 从这里可以明确, 移除指定元素的下一个元素 */
        oldElement = element->next;
        /** 跳跃指定元素的next指向 */
        element->next = element->next->next;
        /** 如果指定元素的next为NULL时, 需要把它置为尾元素 */
        if (NULL == element->next) list->tail = element;
    };
    /** 释放旧元素空间 */
    free(oldElement);
    /** 链表长度-1 */
    list->size--;
    return 0;
}