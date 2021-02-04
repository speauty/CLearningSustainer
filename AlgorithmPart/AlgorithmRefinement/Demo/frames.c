//
// Created by speauty on 2021/2/4.
//

#include "frames.h"

/**
 * 从空闲页帧链表中获取空闲页帧号
 * @param frames 指定页帧
 * @return
 */
int allocFrame(List *frames)
{
    int frameNumber, *data;
    /** 判断是否存在空闲页帧 */
    if (0 == listSize(frames)) {
        return -1;
    } else {
        if (0 != listRemNext(frames, NULL, (void **)&data)) {
            /** 获取空闲页帧失败 */
            return -1;
        } else {
            frameNumber = *data;
            free(data);
        }
    }
    return frameNumber;
}

/**
 * 释放页帧
 * 一旦某个页面从物理内存移除后, 该函数接受一个页帧号并将其放回到空闲页帧链表中。
 * @param frames 指定页帧
 * @param frameNumber 需要释放页帧编号
 * @return
 */
int freeFrame(List *frames, int frameNumber)
{
    int *data;
    /** 分配空间失败 */
    if (NULL == (data = (int *)malloc(sizeof(int)))) return -1;
    *data = frameNumber;
    if (0 != listInsNext(frames, NULL, data)) return -1;
    return 0;
}