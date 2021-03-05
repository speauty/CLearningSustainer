//
// Created by speauty on 2021/3/3.
//

#ifndef DEMO_COMPRESS_H
#define DEMO_COMPRESS_H
/** 数据压缩 */
#include "../data_structure/BiTree.h"

/**
 * 霍夫曼压缩
 * 基于最小冗余编码的压缩算法(如果知道一组数据中符号出现的频率, 就可以用一种特殊的方式来表示符号从而减少数据需要的存储空间)
 * 使用较少的位对出现频率高的符号编码, 用较多的位对出现频率低的符号编码
 */

/** 霍夫曼压缩节点定义 */
typedef struct __HuffNode
{
    unsigned char symbol;
    int freq;
} HuffNode;
/** 霍夫曼表定义 */
typedef struct __HuffCode
{
    /** 默认为1的标志位, 指示此条目是否已经存放一个代码 */
    unsigned char used;
    /** 存放在条目中的霍夫曼编码 */
    unsigned short code;
    /** 编码包含的位数, 编码包含的位数, 每个编码都是一个短整数 */
    /** 当所有的频率调整到可以用一个字节来表示时, 没有编码会大于16位 */
    unsigned char size;
} HuffCode;

/** LZ77签名成员 */
/** 类型标记 */
#define LZ77_TYPE_BITS 1
#define LZ77_WINOFF_BITS 12
/** 缓冲长度 */
#define LZ77_BUFLEN_BITS 5
#define LZ77_NEXT_BITS 8
/** 窗口大小 */
#define LZ77_WINDOW_SIZE 4096
/** 缓冲区大小 */
#define LZ77_BUFFER_SIZE 32
/** LZ77短语标记*/
#define LZ77_PHRASE_BITS (LZ77_TYPE_BITS+LZ77_WINOFF_BITS+LZ77_BUFLEN_BITS+LZ77_NEXT_BITS)
/** LZ77符号标记 */
#define LZ77_SYMBOL_BITS (LZ77_TYPE_BITS+LZ77_NEXT_BITS)


int huffman_compress(const unsigned char *original, unsigned char **compressed, int size);
int huffman_uncompress(const unsigned char *compressed, unsigned char **original);

int lz77_compress(const unsigned char *original, unsigned char **compressed, int size);
int lz77_uncompress(const unsigned char *compressed, unsigned char **original);























#endif //DEMO_COMPRESS_H
