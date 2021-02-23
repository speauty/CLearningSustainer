//
// Created by speauty on 2021/2/23.
//

#include <stdlib.h>
#include <string.h>
#include "BiTree.h"

void bitree_init(BiTree *tree, void (*destroy) (void *data))
{
    tree->size = 0;
    tree->destroy = destroy;
    tree->root = NULL;
}

void bitree_destroy(BiTree *tree)
{
    bitree_rem_left(tree, NULL);
    memset(tree, 0, sizeof(BiTree));
}

/**
 * 插入左子节点
 * @param tree
 * @param node
 * @param data
 * @return
 */
int bitree_ins_left(BiTree *tree, BiTreeNode *node, const void *data)
{
    BiTreeNode *newNode, **position;
    if (NULL == node) {
        if (bitree_size(tree) > 0) return -1;
        position = &tree->root;
    } else {
        if (NULL != bitree_left(node)) return -1;
        position = &node->left;
    }
    if (NULL == (newNode = (BiTreeNode *)malloc(sizeof(BiTreeNode)))) return -1;
    newNode->data = (void *)data;
    newNode->left = NULL;
    newNode->right = NULL;
    *position = newNode;
    tree->size++;
    return 0;
}

/**
 * 插入右子节点
 * @param tree
 * @param node
 * @param data
 * @return
 */
int bitree_ins_right(BiTree *tree, BiTreeNode *node, const void *data)
{
    BiTreeNode *newNode, **position;
    if (NULL == node) {
        if (bitree_size(tree) > 0) return -1;
        position = &tree->root;
    } else {
        if (NULL != bitree_right(node)) return -1;
        position = &node->right;
    }
    if (NULL == (newNode = (BiTreeNode *)malloc(sizeof(BiTreeNode)))) return -1;
    newNode->data = (void *)data;
    newNode->left = NULL;
    newNode->right = NULL;
    *position = newNode;
    tree->size++;
    return 0;
}

/**
 * 删除左子节点
 * @param tree
 * @param node
 */
void bitree_rem_left(BiTree *tree, BiTreeNode *node)
{
    BiTreeNode **position;
    if (bitree_size(tree)) return ;
    if (NULL == node) {
        position = &tree->root;
    } else {
        position = &node->left;
    }

    if (NULL != *position) {
        bitree_rem_left(tree, *position);
        bitree_rem_right(tree, *position);
        if (NULL != tree->destroy) {
            tree->destroy((*position)->data);
        }
        free(*position);
        *position = NULL;
        tree->size--;
    }
}

/**
 * 删除右子节点
 * @param tree
 * @param node
 */
void bitree_rem_right(BiTree *tree, BiTreeNode *node)
{
    BiTreeNode **position;
    if (bitree_size(tree)) return ;
    if (NULL == node) {
        position = &tree->root;
    } else {
        position = &node->right;
    }

    if (NULL != *position) {
        bitree_rem_left(tree, *position);
        bitree_rem_right(tree, *position);
        if (NULL != tree->destroy) {
            tree->destroy((*position)->data);
        }
        free(*position);
        *position = NULL;
        tree->size--;
    }
}

/**
 * 二叉树合并
 * @param merge
 * @param left
 * @param right
 * @param data
 * @return
 */
int bitree_merge(BiTree *merge, BiTree *left, BiTree *right, const void *data)
{
    bitree_init(merge, left->destroy);
    if (0 != bitree_ins_left(merge, NULL, data)) {
        bitree_destroy(merge);
        return -1;
    }
    bitree_root(merge)->left = bitree_root(left);
    bitree_root(merge)->right = bitree_root(right);
    merge->size = merge->size + bitree_size(left) + bitree_size(right);
    left->root = NULL;
    left->size = 0;
    right->root = NULL;
    right->size = 0;
    return 0;
}