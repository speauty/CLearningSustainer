//
// Created by speauty on 2021/2/5.
//

#ifndef DEMO_PAGE_H
#define DEMO_PAGE_H
#include "../data_structure/clist.h"

/** 第二次机会页面置换算法 */
typedef struct __Page
{
    int number;
    int reference
} Page;

int replace_page(CListElement **currentPage);

#endif //DEMO_PAGE_H
