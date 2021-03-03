//
// Created by speauty on 2021/3/3.
//
#include <string.h>
#include "bit.h"

int bit_get(const unsigned char *bits, int pos)
{
    unsigned char mask;
    int i;
    /** 设置需要获取位的掩码(十六进制)(1000 0000) */
    mask = 0x80;
    /**
         * 右移 1000 0000
         * 一次 0100 0000
         * 二次 0010 0000
         */
    for (i = 0; i < (pos % 8); ++i) mask = mask >> 1;
    return (((mask&bits[(int)(pos/8)]) == mask))?1:0;
}

void bit_set(unsigned char *bits, int pos, int state)
{
    unsigned char mask;
    int i;
    mask = 0x80;
    for (i = 0; i < (pos % 8); ++i) mask = mask >> 1;

    if (state) {
        bits[pos/8] = bits[pos/8]|mask;
    } else {
        bits[pos/8] = bits[pos/8]&(~mask);
    }
}

void bit_xor(const unsigned char *bits1, const unsigned char *bits2, unsigned char *bitsX, int size)
{
    int i;
    for (i = 0; i < size; ++i) {
        bit_set(bitsX, i, (bit_get(bits1, i) != bit_get(bits2, i))?1:0);
    }
}

void bit_rot_left(unsigned char *bits, int size, int count)
{
    int fillBit, leaveBit, i, j;
    if (size > 0) {
        for (j = 0; j < count; ++j) {
            for (i = 0; i < ((size - 1) / 8); ++i) {
                /** 保存当前字节的首位 */
                leaveBit = bit_get(&bits[i], 0);
                if (0 == i) {
                    /** 如果是首字节的话, 保存首字节首位 */
                    fillBit = leaveBit;
                } else {
                    /** 将当前移动的位保存到上一字节的末位 */
                    bit_set(&bits[i-1], 7, leaveBit);
                }
                /** 当前字节左移 */
                bits[i] = bits[i]<<1;
            }
            bit_set(bits, size-1, fillBit);
        }
    }
}

