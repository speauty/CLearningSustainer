//
// Created by speauty on 2021/2/18.
//

#ifndef DEMO_SET_H
#define DEMO_SET_H

#include <stdlib.h>
#include "list.h"
/**
 * 集合头文件
 */

typedef List Set;

#define set_destroy list_destroy
#define set_size(set) ((set)->size)

/** 集合初始化 */
void set_init(Set *set, int (*match) (const void *keyFirst, const void keySecond), void (*destroy) (void *data))
/** 集合插入新元素 */
int set_insert(Set *set, const void *data);
/** 集合移除元素 */
int set_remove(Set *set, void **data);
/** 两集合并集存入指定(新)集合 */
int set_union(Set *setResult, const Set *set1, const Set *set2);
/** 两集合交集存入指定(新)集合 */
int set_intersection(Set *setResult, const Set *set1, const Set *set2);
/** 两集合差集存入指定(新)集合 */
int set_difference(Set *setResult, const Set *set1, const Set *set2);
/** 成员判定 */
int set_is_member(const Set *set, const void *data);
/** 子集判定 */
int set_is_subset(const Set *set1, const Set *set2);
/** 相等性判定 */
int set_is_equal(const Set *set1, const Set *set2);

#endif //DEMO_SET_H
