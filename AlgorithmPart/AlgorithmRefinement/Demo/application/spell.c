//
// Created by speauty on 2021/3/2.
//
#include <string.h>
#include "search.h"
#include "spell.h"

static int compare_str(const void *key1, const void *key2)
{
    int result;
    if ((result = strcmp((const char *)key1, (const char *)key2)) > 0) {
        return 1;
    } else if (result < 0) {
        return -1;
    } else {
        return 0;
    }
}

int spell(char (*dictionary)[SPELL_SIZE], int size, const char *word)
{
    if (biSearch(dictionary, word, size, SPELL_SIZE, compare_str) >= 0) {
        return 1;
    } else {
        return 0;
    }
}