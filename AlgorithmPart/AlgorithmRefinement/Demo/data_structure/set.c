//
// Created by speauty on 2021/2/18.
//

#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "set.h"

/**
 * 集合初始化
 * 采用链表实现
 * @param set
 * @param match
 * @param destroy
 */
void set_init(Set *set, int (*match) (const void *keyFirst, const void keySecond), void (*destroy) (void *data))
{
    list_init(set, destroy);
    set->match = match;
}

/**
 * 集合插入元素
 * 需要判断元素是否在集合中, 保证了元素的唯一性
 * @param set
 * @param data
 * @return
 */
int set_insert(Set *set, const void *data)
{
    if (set_is_member(set, data)) return 1;
    return list_ins_next(set, list_tail(set), data);
}

/**
 * 集合移除元素
 * @param set
 * @param data
 * @return
 */
int set_remove(Set *set, void **data)
{
    ListElement  *member, *prev;
    prev = NULL;
    for (member = list_head(set); NULL != member; member = list_next(member)) {
        if (set->match(*data, list_data(member))) break;
        prev = member;
    }
    /** 未找到相应元素 */
    if (NULL == member) return -1;
    return list_rem_next(set, prev, data);
}


/**
 * 两集合并集
 * @param setResult
 * @param set1
 * @param set2
 * @return
 */
int set_union(Set *setResult, const Set *set1, const Set *set2)
{
    ListElement *member;
    /** 初始化结果集合 */
    set_init(setResult, set1->match, NULL);
    /** 将集合1加入结果集合 */
    for (member = list_head(set1); NULL != member; member = list_next(member)) {
        if (0 != list_ins_next(setResult, list_tail(setResult), list_data(member))) {
            set_destroy(setResult);
            return -1;
        }
    }
    /** 将集合2加入结果集合 */
    for (member = list_head(set2); NULL != member; member = list_next(member)) {
        if (set_is_member(set1, list_data(member))) continue;
        if (0 != list_ins_next(setResult, list_tail(setResult), list_data(member))) {
            set_destroy(setResult);
            return -1;
        }
    }
    return 0;
}

/**
 * 两集合交集存入指定(新)集合
 * @param setResult
 * @param set1
 * @param set2
 * @return
 */
int set_intersection(Set *setResult, const Set *set1, const Set *set2)
{
    ListElement *member;
    /** 初始化结果集合 */
    set_init(setResult, set1->match, NULL);

    for (member = list_head(set1); NULL != member; member = list_next(member)) {
        if (set_is_member(set2, list_data(member))) {
            if (0 != list_ins_next(setResult, list_tail(setResult), list_data(member))) {
                set_destroy(setResult);
                return -1;
            }
        }
    }

    return 0;
}

/**
 * 差集
 * @param setResult
 * @param set1
 * @param set2
 * @return
 */
int set_difference(Set *setResult, const Set *set1, const Set *set2)
{
    ListElement *member;
    /** 初始化结果集合 */
    set_init(setResult, set1->match, NULL);
    for (member = list_head(set1); NULL != member; member = list_next(member)) {
        if (!set_is_member(set2, list_data(member))) {
            if (0 != list_ins_next(setResult, list_tail(setResult), list_data(member))) {
                set_destroy(setResult);
                return -1;
            }
        }
    }

    return 0;
}

/**
 * 成员判定
 * @param set
 * @param data
 * @return
 */
int set_is_member(const Set *set, const void *data)
{
    ListElement *member;
    for (member = list_head(set); NULL != member; member = list_next(member)) {
        if (set->match(data, list_data(member))) return 1;
    }
    return 0;
}

/**
 * 子集判定
 * 判断集合1是否为集合2的子集
 * @param set1
 * @param set2
 * @return
 */
int set_is_subset(const Set *set1, const Set *set2)
{
    ListElement  *member;
    if (set_size(set1) > set_size(set2)) return 0;
    for (member = list_head(set); NULL != member; member = list_next(member)) {
        if (!set_is_member(set2, list_data(member))) return 0;
    }
    return 1;
}

/**
 * 相等性判定
 * @param set1
 * @param set2
 * @return
 */
int set_is_equal(const Set *set1, const Set *set2)
{
    if (set_size(set1) != set_size(set2)) return 0;
    return set_is_subset(set1, set2);
}