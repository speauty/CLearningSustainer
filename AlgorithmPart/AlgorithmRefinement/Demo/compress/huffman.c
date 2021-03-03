//
// Created by speauty on 2021/3/3.
//
/** 霍夫曼编码实现 */
#include <limits.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include "bit.h"
#include "../data_structure/BiTree.h"
#include "compress.h"
#include "../data_structure/PriorityQueue.h"

/**
 * 比较两棵二叉树根节点的频率值(HuffNode类型)
 * @param tree1
 * @param tree2
 * @return
 */
static int compare_freq(const void *tree1, const void *tree2)
{
    HuffNode *root1, *root2;
    root1 = (HuffNode *)bitree_data(bitree_root((const BiTree *)tree1));
    root2 = (HuffNode *)bitree_data(bitree_root((const BiTree *)tree2));
    if (root1->freq < root2->freq) {
        return 1;
    } else if (root1->freq > root2->freq) {
        return -1;
    } else {
        return 0;
    }
}

static void destroy_tree(void *tree)
{
    bitree_destroy(tree);
    free(tree);
}

/**
 * 构建霍夫曼树
 * @param freqs 每个符号出现的频率数组
 * @param tree
 * @return
 */
static int build_tree(int *freqs, BiTree **tree)
{
    BiTree *init, *merge, *left, *right;
    PriorityQueue pQueue;
    HuffNode *data;
    int size, c;
    *tree = NULL;
    /** 初始化优先队列 */
    priority_queue_init(&pQueue, compare_freq, destroy_tree);

    for (c = 0; c < UCHAR_MAX; ++c) {
        if (freqs[c] != 0) {
            /** 为当前符号和频率构建二叉树 */
            if (NULL == (init = (BiTree *)malloc(sizeof(BiTree)))) {
                priority_queue_destroy(&pQueue);
                return -1;
            }
            bitree_init(init, free);
            if (NULL == (data = (HuffNode *)malloc(sizeof(HuffNode)))) {
                priority_queue_destroy(&pQueue);
                return -1;
            }
            data->symbol = c;
            data->freq = freqs[c];
            if (bitree_ins_left(init, NULL, data) != 0) {
                free(data);
                bitree_destroy(init);
                free(init);
                priority_queue_destroy(&pQueue);
                return -1;
            }
            /** 将当前二叉树插入到优先队列中 */
            if (priority_queue_insert(&pQueue, init) != 0) {
                bitree_destroy(init);
                free(init);
                priority_queue_destroy(&pQueue);
                return -1;
            }
        }
    }
    /** 通过合并优先队列中的树构建霍夫曼树 */
    size = priority_queue_size(&pQueue);
    for (c = 0; c <= size - 1; ++c) {
        /** 申请存储空间 merge */
        if (NULL == (merge = (BiTree *)malloc(sizeof(BiTree)))) {
            priority_queue_destroy(&pQueue);
            return -1;
        }

        if (priority_queue_extract(&pQueue, (void **)&left) != 0) {
            priority_queue_destroy(&pQueue);
            free(merge);
            return -1;
        }

        if (priority_queue_extract(&pQueue, (void **)&right) != 0) {
            priority_queue_destroy(&pQueue);
            free(merge);
            return -1;
        }

        if (NULL == (data = (HuffNode *)malloc(sizeof(HuffNode)))) {
            priority_queue_destroy(&pQueue);
            free(merge);
            return -1;
        }
        memset(data, 0, sizeof(HuffNode));
        /** 统计即将合并的两树根节点的频率 */
        data->freq = ((HuffNode *)bitree_data(bitree_root(left)))->freq + ((HuffNode *)bitree_data(bitree_root(right)))->freq;
        /** 合并两树 */
        if (bitree_merge(merge, left, right, data) != 0) {
            priority_queue_destroy(&pQueue);
            free(merge);
            return -1;
        }

        if (priority_queue_insert(&pQueue, merge) != 0) {
            priority_queue_destroy(&pQueue);
            bitree_destroy(merge);
            free(merge);
            return -1;
        }
        free(left);
        free(right);
    }
    /** 在优先队列中的最后一树就是霍夫曼树 */
    if (priority_queue_extract(&pQueue, (void **)tree) != 0) {
        priority_queue_destroy(&pQueue);
        return -1;
    } else {
        priority_queue_destroy(&pQueue);
    }
    return 0;
}

/**
 * 构建符号表
 * @param node 节点
 * @param code 编码
 * @param size 大小
 * @param table 符号表
 */
static void build_table(BiTreeNode *node, unsigned short code, unsigned char size, HuffCode *table)
{
    if (!bitree_is_eof(node)) {
        if (!bitree_is_eof(bitree_left(node))) {
            build_table(bitree_left(node), code << 1, size+1, table);
        }

        if (!bitree_is_eof(bitree_right(node))) {
            build_table(bitree_right(node), (code << 1)|0x0001, size+1, table);
        }

        if (bitree_is_eof(bitree_left(node)) && bitree_is_eof(bitree_right(node))) {
            /** 将编码转换为大端 */
            code = htons(code);
            /** 在叶子节点时, 将当前编码添加到符号表 */
            table[((HuffNode *)bitree_data(node))->symbol].used = 1;
            table[((HuffNode *)bitree_data(node))->symbol].code = code;
            table[((HuffNode *)bitree_data(node))->symbol].size = size;
        }
    }
}

int huffman_compress(const unsigned char *original, unsigned char **compressed, int size)
{
    BiTree  *tree;
    HuffCode table[UCHAR_MAX+1];
    int freqs[UCHAR_MAX+1], max, scale, hSize, iPos, oPos, cPos, c, i;
    unsigned char *comp, *temp;
    *compressed = NULL;
    for (c = 0; c <= UCHAR_MAX; ++c) freqs[c] = 0;
    iPos = 0;
    if (size > 0) {
        while (iPos < size) {
            freqs[original[iPos]]++;
            iPos++;
        }
    }
    max = UCHAR_MAX;
    for (c = 0; c <= UCHAR_MAX; ++c) if (freqs[c] > max) max = freqs[c];
    /** 计算出现频率 */
    for (c = 0; c <= UCHAR_MAX; ++c) {
        scale = (int) (freqs[c]/((double)max/(double)UCHAR_MAX));
        if (scale == 0 && freqs[c] != 0) {
            freqs[c] = 1;
        } else {
            freqs[c] = scale;
        }
    }
    if (build_tree(freqs, &tree) != 0) return -1;
    for (c = 0; c <= UCHAR_MAX; ++c) memset(&table[c], 0, sizeof(HuffCode));
    build_table(bitree_root(tree), 0x0000, 0, table);
    bitree_destroy(tree);
    free(tree);
    hSize = sizeof(int)+(UCHAR_MAX+1);
    if (NULL == (comp = (unsigned char *)malloc(hSize))) return -1;
    memcpy(comp, &size, sizeof(int));
    for (c = 0; c <= UCHAR_MAX; ++c) {
        comp[sizeof(int)+c] = (unsigned char)freqs[c];
    }
    iPos = 0;
    oPos = hSize*8;
    while (iPos < size) {
        /** 获取原始数据中的下一个符号 */
        c = original[iPos];
        /** 将该符号对应的编码写入压缩数据缓存 */
        for (i = 0; i < table[c].size; ++i) {
            if (oPos%8 == 0) {
                if (NULL == (temp = (unsigned char *)realloc(comp, (oPos/8)+1))) {
                    free(comp);
                    return -1;
                }
                comp = temp;
            }
            cPos = (sizeof(short)*8)-table[c].size+i;
            bit_set(comp, oPos, bit_get((unsigned char *)&table[c].code, cPos));
            oPos++;
        }
        iPos++;
    }
    *compressed = comp;
    return ((oPos-1)/8)+1;
}

int huffman_uncompress(const unsigned char *compressed, unsigned char **original)
{
    BiTree *tree;
    BiTreeNode *node;
    int freqs[UCHAR_MAX+1], hSize, size, iPos, oPos, state, c;
    unsigned char *orig, *temp;
    *original = orig = NULL;
    hSize = sizeof(int)+(UCHAR_MAX+1);
    memcpy(&size, compressed, sizeof(int));
    for (c = 0; c <= UCHAR_MAX; ++c) freqs[c] = compressed[sizeof(int)+c];
    if (build_tree(freqs, &tree) != 0) return -1;
    iPos = hSize*8;
    oPos = 0;
    node = bitree_root(tree);
    while (oPos < size) {
        state = bit_get(compressed, iPos);
        iPos++;
        if (state == 0) {
            if (bitree_is_eof(node) || bitree_is_eof(bitree_left(node))) {
                bitree_destroy(tree);
                free(tree);
                return -1;
            } else {
                node = bitree_left(node);
            }
        } else {
            if (bitree_is_eof(node) || bitree_is_eof(bitree_right(node))) {
                bitree_destroy(tree);
                free(tree);
                return -1;
            } else {
                node = bitree_right(node);
            }
        }

        if (bitree_is_eof(bitree_left(node)) && bitree_is_eof(bitree_right(node))) {
            if (oPos > 0) {
                if (NULL == (temp = (unsigned char *)realloc(orig, oPos+1))) {
                    bitree_destroy(tree);
                    free(tree);
                    free(orig);
                    return -1;
                }
                orig = temp;
            } else {
                if (NULL == (orig = (unsigned char *)malloc(1))) {
                    bitree_destroy(tree);
                    free(tree);
                    return -1;
                }
            }
            orig[oPos] = ((HuffNode *)bitree_data(node))->symbol;
            oPos++;
            node = bitree_root(tree);
        }
    }
    bitree_destroy(tree);
    free(tree);
    *original = orig;
    return oPos;
}





























