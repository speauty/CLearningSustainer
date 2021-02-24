//
// Created by speauty on 2021/2/23.
//

#ifndef DEMO_BSTREE_H
#define DEMO_BSTREE_H

#include "BiTree.h"

/** 左倾斜 */
#define AVL_LEFT_HEAVY 1
/** 平衡 */
#define AVL_BALANCED 0
/** 右倾斜 */
#define AVL_RIGHT_HEAVY -1

/** AVL树: 所有结点的平衡因子的绝对值都不超过 1 的二叉树 */
/** AVL树节点 */
typedef struct __AVLNode
{
    void *data;
    int hidden;
    /** 平衡因子: 某个结点的左子树的高度减去右子树的高度得到的差值 */
    int factor;
} AVLNode;

typedef BiTree BSTree;
#define bstree_size(tree) ((tree)->size)

void bstree_init(BSTree *tree, int (*compare) (const void *key1, const void *key2), void (*destroy) (void *data));
void bstree_destroy(BSTree *tree);
int bstree_insert(BSTree *tree, const void *data);
int bstree_remove(BSTree *tree, const void *data);
int bstree_lookup(BSTree *tree, void **data);

#endif //DEMO_BSTREE_H
