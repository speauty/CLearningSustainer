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
 * 比较两棵二叉树根节点的频率值(data为HuffNode类型)
 * @param tree1
 * @param tree2
 * @return
 */
static int compare_freq(const void *tree1, const void *tree2)
{
    HuffNode *root1, *root2;
    /** 根据根节点的freq比较大小 */
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
static int build_tree(const int *freqs, BiTree **tree)
{
    BiTree *init, *merge, *left, *right;
    PriorityQueue pQueue;
    HuffNode *data;
    int size, c;
    *tree = NULL;
    /** 初始化优先队列 */
    priority_queue_init(&pQueue, compare_freq, destroy_tree);

    /** UCHAR_MAX: 无符号char的最大值(255), 一个字节 */
    /** 为当前符号和频率构建二叉树 */
    for (c = 0; c < UCHAR_MAX; ++c) {
        /** 该字符对应的频率是否为0 */
        if (freqs[c] != 0) {
            /** 申请存储空间(二叉树) */
            if (NULL == (init = (BiTree *)malloc(sizeof(BiTree)))) {
                priority_queue_destroy(&pQueue);
                return -1;
            }
            bitree_init(init, free);
            /** 为节点data申请储存空间(HuffNode) */
            if (NULL == (data = (HuffNode *)malloc(sizeof(HuffNode)))) {
                priority_queue_destroy(&pQueue);
                return -1;
            }
            /** 符号 */
            data->symbol = c;
            /** 该符号出现的频率 */
            data->freq = freqs[c];
            /** 作为根节点插入到二叉树init中 */
            if (bitree_ins_left(init, NULL, data) != 0) {
                free(data);
                bitree_destroy(init);
                free(init);
                priority_queue_destroy(&pQueue);
                return -1;
            }
            /** 将当前二叉树init(只含有一个根节点的二叉树)插入到优先队列(大根堆)中 */
            if (priority_queue_insert(&pQueue, init) != 0) {
                bitree_destroy(init);
                free(init);
                priority_queue_destroy(&pQueue);
                return -1;
            }
        }
    }
    /** 通过合并优先队列中的树构建霍夫曼树 */
    /** 获取优先队列的长度 */
    size = priority_queue_size(&pQueue);
    /** left->freq > right->freq 并且size-1表示队列中只保留最后一个, 即霍夫曼树 */
    for (c = 0; c <= size - 1; ++c) {
        /** 申请存储空间 merge */
        if (NULL == (merge = (BiTree *)malloc(sizeof(BiTree)))) {
            priority_queue_destroy(&pQueue);
            return -1;
        }
        /** 释放优先队列头元素, 将数据指针保存到left */
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
        /** data->symbol 未设置 */
        /** 合并两树 */
        if (bitree_merge(merge, left, right, data) != 0) {
            priority_queue_destroy(&pQueue);
            free(merge);
            return -1;
        }
        /** 再将合并好的二叉放回队列中, 继续下一轮 */
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
 * @param size 编码的位数
 * @param table 符号表
 */
static void build_table(BiTreeNode *node, unsigned short code, unsigned char size, HuffCode *table)
{
    if (!bitree_is_eof(node)) {
        /** 选择左分支, 编码末尾追加0 */
        if (!bitree_is_eof(bitree_left(node))) {
            build_table(bitree_left(node), code << 1, size+1, table);
        }
        /** 选择左分支, 编码末尾追加1 */
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

/**
 * 霍夫曼压缩
 * @param original 原始字符树组
 * @param compressed 压缩后的字符树组
 * @param size
 * @return
 */
int huffman_compress(const unsigned char *original, unsigned char **compressed, int size)
{
    BiTree  *tree;
    HuffCode table[UCHAR_MAX+1];
    /**
     * iPos 保存原始数据中正在处理的当前字节
     * oPos 保存向压缩数据缓冲区写入的当前位
     * max  出现字符的最大频率
     * scale 频率的缩放
     */
    int freqs[UCHAR_MAX+1], max, scale, hSize, iPos, oPos, cPos, c, i;
    unsigned char *comp, *temp;
    *compressed = NULL;
    /** 初始化字符频率树组 */
    for (c = 0; c <= UCHAR_MAX; ++c) freqs[c] = 0;
    iPos = 0;
    /** 计算字符出现的频率 */
    if (size > 0) {
        while (iPos < size) {
            /** original[iPos] 获取到对应字符 */
            freqs[original[iPos]]++;
            iPos++;
        }
    }
    max = UCHAR_MAX;
    /** 计算字符最大频率 */
    for (c = 0; c <= UCHAR_MAX; ++c) if (freqs[c] > max) max = freqs[c];
    /** 频率缩放 */
    for (c = 0; c <= UCHAR_MAX; ++c) {
        /**
         * 懂了, 缩放
         * scale 最大值为UCHAR_MAX, 也就是说出现频率越高, 也靠近二叉树顶端
         */
        scale = (int) (freqs[c]/((double)max/(double)UCHAR_MAX));
        /** 如果缩放后的频率为0, 并且该字符原始频率非0, 将其频率设置为1, 否则设置为缩放频率 */
        if (scale == 0 && freqs[c] != 0) {
            freqs[c] = 1;
        } else {
            freqs[c] = scale;
        }
    }
    if (build_tree(freqs, &tree) != 0) return -1;
    /** 格式化符号表存储空间 */
    for (c = 0; c <= UCHAR_MAX; ++c) memset(&table[c], 0, sizeof(HuffCode));
    /** 从根节点开始构建符号表 */
    build_table(bitree_root(tree), 0x0000, 0, table);
    bitree_destroy(tree);
    free(tree);
    /** hSize总共字符数 */
    hSize = sizeof(int)+(UCHAR_MAX+1);
    if (NULL == (comp = (unsigned char *)malloc(hSize))) return -1;
    /** 将字符的个数拷贝到comp中 前四字节 */
    memcpy(comp, &size, sizeof(int));
    /** 随后写入字符的频率 */
    for (c = 0; c <= UCHAR_MAX; ++c) {
        comp[sizeof(int)+c] = (unsigned char)freqs[c];
    }
    iPos = 0;
    /** 总共位数 */
    oPos = hSize*8;
    while (iPos < size) {
        /** 获取原始字符树组中iPos位置的字符 */
        c = original[iPos];
        /** 将该符号对应的编码写入压缩数据缓存 */
        /** i循环 向后读取/写入 */
        for (i = 0; i < table[c].size; ++i) {
            /** 重新分配comp存储空间(应该是刚好填充完一个字节 8bit) */
            if (oPos%8 == 0) {
                /** 始终比上轮多一个长度 */
                if (NULL == (temp = (unsigned char *)realloc(comp, (oPos/8)+1))) {
                    free(comp);
                    return -1;
                }
                comp = temp;
            }
            /** 计算读取起点位 */
            cPos = (sizeof(short)*8)-table[c].size+i;
            /** 设置当前oPos位 */
            bit_set(comp, oPos, bit_get((unsigned char *)&table[c].code, cPos));
            oPos++;
        }
        iPos++;
    }
    *compressed = comp;
    /** 返回压缩后数据的字节数 */
    /** oPos会多加一次/8 获得字节数 为什么会+1(在重新分配comp存储空间时, 最后一轮, 会多出一字节) */
    return ((oPos-1)/8)+1;
}

int huffman_uncompress(const unsigned char *compressed, unsigned char **original)
{
    BiTree *tree;
    BiTreeNode *node;
    /**
     * iPos 保存向压缩数据缓冲区写入的当前位
     * oPos 保存写入恢复数据缓冲区中的当前字节
     */
    int freqs[UCHAR_MAX+1], hSize, size, iPos, oPos, state, c;
    unsigned char *orig, *temp;
    *original = orig = NULL;
    hSize = sizeof(int)+(UCHAR_MAX+1);
    /** 从压缩字符树组中获取字符个数 */
    memcpy(&size, compressed, sizeof(int));
    /** 计算压缩字符树组中字符出现频率 */
    for (c = 0; c <= UCHAR_MAX; ++c) freqs[c] = compressed[sizeof(int)+c];
    /** 构建霍夫曼树 */
    if (build_tree(freqs, &tree) != 0) return -1;
    iPos = hSize*8;
    oPos = 0;
    /** 从霍夫曼树的根开始, 只要遇到位数0, 就选择左分支; 只要遇到位数1, 就选择右分支 */
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
        /** 一旦到达叶子结点, 就获取一个符号的霍夫曼编码 */
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
            /** 获取对应符号 */
            orig[oPos] = ((HuffNode *)bitree_data(node))->symbol;
            oPos++;
            /** 重新回到霍夫曼树的根节点, 进行下一个字符的寻找 */
            node = bitree_root(tree);
        }
    }
    bitree_destroy(tree);
    free(tree);
    *original = orig;
    /** 返回恢复后数据的字节数 */
    return oPos;
}





























