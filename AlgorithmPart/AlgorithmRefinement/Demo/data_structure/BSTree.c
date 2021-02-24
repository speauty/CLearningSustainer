//
// Created by speauty on 2021/2/23.
//

#include <stdlib.h>
#include <string.h>
#include "BSTree.h"

static void destroy_right(BSTree *tree, BiTreeNode *node);

/**
 * 左旋转
 * @param node
 */
static void rotate_left(BiTreeNode **node)
{
    BiTreeNode *left, *grandChild;
    /** 当前节点的左子结点 */
    left = bitree_left(*node);
    /** 左边重*/
    if (AVL_LEFT_HEAVY == ((AVLNode *)bitree_data(left))->factor) {
        /** LL旋转 */
        /** 把左子节点的右子节点挂到当前节点的左子结点位置 */
        bitree_left(*node) = bitree_right(left);
        /** 把当前节点挂到左子结点的右子结点位置 */
        bitree_right(left) = *node;
        /** 调整平衡因子 */
        ((AVLNode *)bitree_data(*node))->factor = AVL_BALANCED;
        ((AVLNode *)bitree_data(left))->factor = AVL_BALANCED;
        /** 把左字节点挂到当前节点位置 */
        *node = left;
    } else {
        /** LR旋转 */
        /** 孙节点: 左字节点的右子结点 */
        grandChild = bitree_right(left);
        /** 将孙节点的左子结点挂到左子结点的右子结点位置 */
        bitree_right(left) = bitree_left(grandChild);
        /** 将左子结点挂到孙节点的左子结点位置 */
        bitree_left(grandChild) = left;
        /** 将孙节点的右子结点挂到当前节点的左子结点位置 */
        bitree_left(*node) = bitree_right(grandChild);
        /** 将当前节点挂到孙节点的右子结点位置 */
        bitree_right(grandChild) = *node;
        /** 调整平衡因子 */
        switch (((AVLNode *)bitree_data(grandChild))->factor) {
            case AVL_LEFT_HEAVY:
                ((AVLNode *)bitree_data(*node))->factor = AVL_RIGHT_HEAVY;
                ((AVLNode *)bitree_data(left))->factor = AVL_BALANCED;
                break;
            case AVL_BALANCED:
                ((AVLNode *)bitree_data(*node))->factor = AVL_BALANCED;
                ((AVLNode *)bitree_data(left))->factor = AVL_BALANCED;
                break;
            case AVL_RIGHT_HEAVY:
                ((AVLNode *)bitree_data(*node))->factor = AVL_BALANCED;
                ((AVLNode *)bitree_data(left))->factor = AVL_LEFT_HEAVY;
                break;
        }
        ((AVLNode *)bitree_data(grandChild))->factor = AVL_BALANCED;
        /** 将孙节点挂到当前节点位置 */
        *node = grandChild;
    }

}

/**
 * 右旋转
 * @param node
 */
static void rotate_right(BiTreeNode **node)
{
    BiTreeNode *right, *grandChild;
    right = bitree_right(*node);

    if (AVL_RIGHT_HEAVY == ((AVLNode *)bitree_data(right))->factor) {
        /** RR旋转 */
        /** 将右子结点的左子结点挂到当前节点的右子结点位置 */
        bitree_right(*node) = bitree_left(right);
        /** 将当前节点挂到右子结点的左子结点位置 */
        bitree_left(right) = *node;
        ((AVLNode *)bitree_data(*node))->factor = AVL_BALANCED;
        ((AVLNode *)bitree_data(right))->factor = AVL_BALANCED;
        /** 将右子结点挂到当前节点 */
        *node = right;
    } else {
        /** RL旋转 */
        /** 孙节点: 右子结点的左子结点 */
        grandChild = bitree_left(right);
        /** 将孙节点的右子结点挂到右子结点的左子结点位置(该位置已闲置出来) */
        bitree_left(right) = bitree_right(grandChild);
        /** 将右子结点挂到孙节点的右子结点位置 */
        bitree_right(grandChild) = right;
        /** 将孙子结点的左子结点挂到当前节点的右子结点位置 */
        bitree_right(*node) = bitree_left(grandChild);
        /** 将当前节点挂到孙子结点的左子结点位置 */
        bitree_left(grandChild) = *node;
        /** 调整平衡因子 */
        switch (((AVLNode *)bitree_data(grandChild))->factor) {
            case AVL_LEFT_HEAVY:
                ((AVLNode *)bitree_data(*node))->factor = AVL_BALANCED;
                ((AVLNode *)bitree_data(right))->factor = AVL_RIGHT_HEAVY;
                break;
            case AVL_BALANCED:
                ((AVLNode *)bitree_data(*node))->factor = AVL_BALANCED;
                ((AVLNode *)bitree_data(right))->factor = AVL_BALANCED;
                break;
            case AVL_RIGHT_HEAVY:
                ((AVLNode *)bitree_data(*node))->factor = AVL_LEFT_HEAVY;
                ((AVLNode *)bitree_data(right))->factor = AVL_BALANCED;
                break;
        }
        ((AVLNode *)bitree_data(grandChild))->factor = AVL_BALANCED;
        /** 将孙子节点挂到当前节点 */
        *node = grandChild;
    }
}

/**
 * 删除指定节点的左子节点
 * @param tree
 * @param node
 */
static void destroy_left(BSTree *tree, BiTreeNode *node)
{
    BiTreeNode **position;
    if (0 == bstree_size(tree)) return ;
    /** 定位需要删除节点的起点 */
    if (NULL == node) {
        position = &tree->root;
    } else {
        /** 从左子结点开始 */
        position = &node->left;
    }

    if (NULL != *position) {
        destroy_left(tree, *position);
        destroy_right(tree, *position);
        if (NULL != tree->destroy) {
            tree->destroy(((AVLNode *)(*position)->data)->data);
        }
        free((*position)->data);
        free(*position);
        *position = NULL;
        tree->size--;
    }
}

/**
 * 删除指定节点的右子结点
 * @param tree
 * @param node
 */
static void destroy_right(BSTree *tree, BiTreeNode *node)
{
    BiTreeNode **position;
    if (0 == bstree_size(tree)) return ;
    if (NULL == node) {
        position = &tree->root;
    } else {
        position = &node->right;
    }

    if (NULL == *position) {
        destroy_left(tree, *position);
        destroy_right(tree, *position);
        if (NULL != tree->destroy) {
            tree->destroy(((AVLNode *)(*position)->data)->data);
        }
        free((*position)->data);
        free(*position);
        *position = NULL;
        tree->size--;
    }
}

/**
 * 插入节点
 * @param tree
 * @param node
 * @param data
 * @param balanced
 * @return
 */
static int insert(BSTree *tree, BiTreeNode **node, const void *data, int *balanced)
{
    AVLNode *avlData;
    int compareResult, result;
    /** 判断是不是空节点 */
    if (bitree_is_eof(*node)) {
        if (NULL == (avlData = (AVLNode *)malloc(sizeof(AVLNode)))) {
            return -1;
        }
        avlData->factor = AVL_BALANCED;
        avlData->hidden = 0;
        avlData->data = (void *)data;
        return bitree_ins_left(tree, *node, avlData);
    } else {
        compareResult = tree->compare(data, ((AVLNode *)bitree_data(*node))->data);

        if (compareResult < 0) {
            /** 放在*node的左边 */
            if (bitree_is_eof(bitree_left(*node))) {
                if (NULL == (avlData = (AVLNode *)malloc(sizeof(AVLNode)))) {
                    return -1;
                }
                avlData->factor = AVL_BALANCED;
                avlData->hidden = 0;
                avlData->data = (void *)data;
                if (0 != bitree_ins_left(tree, *node, avlData)) return -1;
                *balanced = 0;
            } else {
                /** 在递归处理 */
                if (0 != (result = insert(tree, &bitree_left(*node), data, balanced))) {
                    return result;
                }
            }

            /** 确保树是平衡的 */
            if (!(*balanced)) {
                switch (((AVLNode *)bitree_data(*node))->factor) {
                    case AVL_LEFT_HEAVY:
                        rotate_left(node);
                        *balanced = 1;
                        break;
                    case AVL_BALANCED:
                        ((AVLNode *)bitree_data(*node))->factor = AVL_LEFT_HEAVY;
                        break;
                    case AVL_RIGHT_HEAVY:
                        ((AVLNode *)bitree_data(*node))->factor = AVL_BALANCED;
                        *balanced = 1;
                        break;
                }
            }
        } else if (compareResult > 0) {
            /** 放在*node的右边 */
            if (bitree_is_eof(bitree_right(*node))) {
                if (NULL == (avlData = (AVLNode *)malloc(sizeof(AVLNode)))) {
                    return -1;
                }
                avlData->factor = AVL_BALANCED;
                avlData->hidden = 0;
                avlData->data = (void *)data;
                if (0 != bitree_ins_right(tree, *node, avlData)) return -1;
                *balanced = 0;
            } else {
                if (0 != (result = insert(tree, &bitree_right(*node), data, balanced))) {
                    return result;
                }
            }

            /** 确保树是平衡的 */
            if (!(*balanced)) {
                switch (((AVLNode *)bitree_data(*node))->factor) {
                    case AVL_LEFT_HEAVY:
                        ((AVLNode *)bitree_data(*node))->factor = AVL_BALANCED;
                        *balanced = 1;
                        break;
                    case AVL_BALANCED:
                        ((AVLNode *)bitree_data(*node))->factor = AVL_RIGHT_HEAVY;
                        break;
                    case AVL_RIGHT_HEAVY:
                        rotate_right(node);
                        *balanced = 1;
                        break;
                }
            }
        } else {
            if (!((AVLNode *)bitree_data(*node))->hidden) {
                return 1;
            } else {
                if (NULL != tree->destroy) {
                    tree->destroy(((AVLNode *)bitree_data(*node))->data);
                }
                ((AVLNode *)bitree_data(*node))->data = (void *)data;
                ((AVLNode *)bitree_data(*node))->hidden = 0;
                *balanced = 1;
            }
        }
    }
    return 0;
}

/**
 * 隐藏节点[指定节点作为参考]
 * @param tree
 * @param node
 * @param data
 * @return
 */
static int hide(BSTree *tree, BiTreeNode *node, const void *data)
{
    int compareResult, result;
    if (bitree_is_eof(node)) return -1;
    compareResult = tree->compare(data, ((AVLNode *)bitree_data(node))->data);
    if (compareResult < 0) {
        /** 在左边 */
        result = hide(tree, bitree_left(node), data);
    } else if (compareResult > 0) {
        /** 在右边 */
        result = hide(tree, bitree_right(node), data);
    } else {
        /** 把节点标记为隐藏 */
        ((AVLNode *)bitree_data(node))->hidden = 1;
        result = 0;
    }
    return result;
}

/**
 * 查询节点
 * @param tree
 * @param node
 * @param data
 * @return
 */
static int lookup(BSTree *tree, BiTreeNode *node, void **data)
{
    int compareResult, result;
    if (bitree_is_eof(node)) return -1;
    compareResult = tree->compare(*data, ((AVLNode *)bitree_data(node))->data);
    if (compareResult < 0) {
        result = lookup(tree, bitree_left(node), data);
    } else if (compareResult > 0) {
        result = lookup(tree, bitree_right(node), data);
    } else {
        if (!((AVLNode *)bitree_data(node))->hidden) {
            *data = ((AVLNode *)bitree_data(node))->data;
            result = 0;
        } else {
            return -1;
        }
    }
    return result;
}

void bstree_init(BSTree *tree, int (*compare) (const void *key1, const void *key2), void (*destroy) (void *data))
{
    bitree_init(tree, destroy);
    tree->compare = compare;
}
void bstree_destroy(BSTree *tree)
{
    destroy_left(tree, NULL);
    memset(tree, 0, sizeof(BSTree))
}

int bstree_insert(BSTree *tree, const void *data)
{
    int *balanced = 0;
    return insert(tree, &bitree_root(tree), data, &balanced);
}

int bstree_remove(BSTree *tree, const void *data)
{
    return hide(tree, bitree_root(tree), data);
}

int bstree_lookup(BSTree *tree, void **data)
{
    return lookup(tree, bitree_root(tree), data);
}
