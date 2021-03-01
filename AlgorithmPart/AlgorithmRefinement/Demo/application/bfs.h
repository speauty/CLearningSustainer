//
// Created by speauty on 2021/3/1.
//

#ifndef DEMO_BFS_H
#define DEMO_BFS_H
#include "../data_structure/graph.h"
#include "../data_structure/list.h"

/**
 * 广度优先搜索的bfs
 * 确定两个结点之间的最小跳数
 */

/***
 * 顶点
 */
typedef struct __BfsVertex
{
    /** 指向图中顶点的数据域的指针 */
    void *data;
    /** 搜索过程中维护顶点的颜色 */
    VertexColor color;
    /** 巍从起始节点开始到顶点的跳数统计 */
    int hops;
} BfsVertex;

int bfs(Graph *graph, BfsVertex *start, List *hops);

#endif //DEMO_BFS_H
