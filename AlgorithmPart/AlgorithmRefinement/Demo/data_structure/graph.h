//
// Created by speauty on 2021/2/26.
//

#ifndef DEMO_GRAPH_H
#define DEMO_GRAPH_H
#include <stdlib.h>
#include "list.h"
#include "set.h"

/** 邻接表链表 */
typedef struct __AdjList
{
    /** 顶点 */
    void *vertex;
    /** 与该顶点相邻接的一个顶点集合 */
    Set adjacent;
} AdjList;

/** 图结构 */
typedef struct __Graph
{
    /** 图中顶点的个数 */
    int vCount;
    /** 图中边的个数 */
    int eCount;
    int (*match) (const void *key1, const void *key2);
    void (*destroy) (void *data);
    /** 邻接表链表 */
    List adjLists;
} Graph;

/** 顶点颜色枚举类型 */
typedef enum __VertexColor{white, gray, black} VertexColor;

#define graph_adjList(graph) ((graph)->adjLists)
#define graph_vCount(graph) ((graph)->vCount)
#define graph_eCount(graph) ((graph)->eCount)

void graph_init(Graph *graph, int (*match) (const void *key1,const void *key2), void (*destroy) (void *data));
void graph_destroy(Graph *graph);
/**
 * 将一个顶点插入图中
 * 将一个AdjList结构体插入邻接表链表结构中, 并将AdjList结构体中的vertex成员指向由调用者传入的数据
 * @param graph
 * @param data
 * @return
 */
int graph_ins_vertex(Graph *graph, const void *data);
/**
 * 将一条边插入图中
 * @param graph
 * @param data1
 * @param data2
 * @return
 */
int graph_ins_edge(Graph *graph, const void *data1, const void *data2);
int graph_rem_vertex(Graph *graph, void **data);
int graph_rem_edge(Graph *graph, void *data1, void **data2);
int graph_adj_list(const Graph *graph, const void *data, AdjList **adjList);
/**
 * 判断指定的顶点data1是否与另一个顶点data2有邻接关系
 * @param graph
 * @param data1
 * @param data2
 * @return
 */
int graph_is_adjacent(const Graph *graph, const void *data1, const void *data2);

#endif //DEMO_GRAPH_H