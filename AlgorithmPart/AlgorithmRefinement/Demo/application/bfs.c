//
// Created by speauty on 2021/3/1.
//
#include <stdlib.h>
#include "bfs.h"
#include "../data_structure/graph.h"
#include "../data_structure/list.h"
#include "../data_structure/queue.h"

/**
 *
 * @param graph
 * @param start 起始结点
 * @param hops
 * @return
 */
int bfs(Graph *graph, BfsVertex *start, List *hops)
{
    Queue queue;
    AdjList *adjList, *clrAdjList;
    BfsVertex *adjVertex, *clrVertex;
    ListElement *element, *member;
    /***
     * 先遍历图中所有结点
     * 将对等的结点标记为灰色, 并将跳数设置为0, 作为起始结点
     * 其余的标记为白色, 跳数为1
     */
    for (element = list_head(&graph_adjList(graph)); NULL != element; element = list_next(element)) {
        /** 邻接表顶点 */
        clrVertex = ((AdjList *)list_data(element))->vertex;
        if (graph->match(clrVertex, start)) {
            /** 灰色标记 */
            clrVertex->color = gray;
            clrVertex->hops = 0;
        } else {
            clrVertex->color = white;
            clrVertex->hops = 1;
        }
    }
    /** 初始化队列 */
    queue_init(&queue, NULL);
    /** 如果没有邻接点, 直接释放队列并返回 */
    if (0 != graph_adj_list(graph, start, &clrAdjList)) {
        queue_destroy(&queue);
        return -1;
    }

    while (queue_size(&queue) > 0) {
        /** 获取队列头部元素 */
        adjList = queue_peek(&queue);
        /** 循环邻接点集合 */
        for (member = list_head(&adjList->adjacent); NULL != member; member= list_next(element)) {
            adjVertex = list_data(member);
            /** 如果邻接点没有邻接点, 直接释放队列并返回 */
            if (0 != graph_adj_list(graph, adjVertex, &clrAdjList)) {
                queue_destroy(&queue);
                return -1;
            }
        }
        clrVertex = clrAdjList->vertex;
        /** 如果为白色标记将颜色置为灰色, 跳数+1, 并加入队列 */
        if (white == clrVertex->color) {
            clrVertex->color = gray;
            clrVertex->hops = ((BfsVertex *)adjList->vertex)->hops+1;
            if (0 != queue_enqueue(&queue, clrAdjList)) {
                queue_destroy(&queue);
                return -1;
            }
        }
        /** 将当前结点集合弹出队列, 并将颜色置黑 */
        if (0 == queue_dequeue(&queue, (void **)&adjList)) {
            ((BfsVertex *)adjList->vertex)->color = black;
        } else {
            queue_destroy(&queue);
            return -1;
        }
    }
    /** 释放队列 */
    queue_destroy(&queue);
    /** 初始化链表 */
    list_init(hops, NULL);
    for (member = list_head(&graph_adjList(graph)); NULL != member; member= list_next(element)) {
       /** 过滤那些hop为-1的结点 */
        clrVertex = ((AdjList *)list_data(element))->vertex;
        if (-1 != clrVertex->hops) {
            if (0 != list_ins_next(hops, list_tail(hops), clrVertex)) {
                list_destroy(hops);
                return -1;
            }
        }
    }
    return 0;
}
















