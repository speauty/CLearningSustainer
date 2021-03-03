//
// Created by speauty on 2021/3/3.
//

#ifndef DEMO_BIT_H
#define DEMO_BIT_H

/**
 * 获取缓冲区bits中处于位置pos的位的状态
 * 缓冲区最左边的位置为0
 * @param bits 字节数组
 * @param pos
 * @return
 */
int bit_get(const unsigned char *bits, int pos);

/**
 * 设置缓冲区bits中处于位置pos的位的状态
 * @param bits 字节数组
 * @param pos
 * @param state 状态值必须为0或1
 */
void bit_set(unsigned char *bits, int pos, int state);

/**
 * 位的异或运算
 * @param bits1 字节数组
 * @param bits2 字节数组
 * @param bitsX 结果值
 * @param size 位的数量
 */
void bit_xor(const unsigned char *bits1, const unsigned char *bits2, unsigned char *bitsX, int size);

/**
 * 向左轮转
 * 在移动字节的过程中, 将前一个字节最右边的位移到当前字节的最左边
 * 当处理到最后一个字节时, 将其最右边的位移动到首字节的最高位上
 * @param bits 字节数组
 * @param size 位数量
 * @param count 左移多少位
 */
void bit_rot_left(unsigned char *bits, int size, int count);

#endif //DEMO_BIT_H
