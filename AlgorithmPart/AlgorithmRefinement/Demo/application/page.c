//
// Created by speauty on 2021/2/5.
//

#include "page.h"
#include "../data_structure/clist.h"

int replace_page(CListElement **currentPage)
{
    while (0 != ((Page *)((*currentPage)->data))->reference) {
        ((Page *)(*currentPage)->data)->reference = 0;
        *currentPage = clist_next(*currentPage);
    }
    return ((Page *)(*currentPage)->data)->number;
}
