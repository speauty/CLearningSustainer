//
// Created by speauty on 2021/2/23.
//

#ifndef DEMO_BITREE_H
#define DEMO_BITREE_H

/** 二叉树节点 */
typedef struct __BiTreeNode
{
    void *data;
    struct __BiTreeNode *left;
    struct __BiTreeNode *right;
} BiTreeNode;

/** 二叉树 */
typedef struct __BiTree
{
    int size;
    int (*compare) (const void *key1, const void *key2);
    void (*destroy) (void *data);
    BiTreeNode *root;
} BiTree;

#define bitree_size(tree) ((tree)->size)
#define bitree_root(tree) ((tree)->root)
#define bitree_is_eof(node) ((node) == NULL)
#define bitree_is_leaf(node) ((NULL == (node)->left) && NULL == (node)->right)
#define bitree_data(node) ((node)->data)
#define bitree_left(node) ((node)->left)
#define bitree_right(node) ((node)->right)

void bitree_init(BiTree *tree, void (*destroy) (void *data));
void bitree_destroy(BiTree *tree);
int bitree_ins_left(BiTree *tree, BiTreeNode *node, const void *data);
int bitree_ins_right(BiTree *tree, BiTreeNode *node, const void *data);
void bitree_rem_left(BiTree *tree, BiTreeNode *node);
void bitree_rem_right(BiTree *tree, BiTreeNode *node);
int bitree_merge(BiTree *merge, BiTree *left, BiTree *right, const void *data);

#endif //DEMO_BITREE_H
