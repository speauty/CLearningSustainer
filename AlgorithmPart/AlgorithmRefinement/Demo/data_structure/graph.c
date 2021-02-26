//
// Created by speauty on 2021/2/26.
//
#include <stdlib.h>
#include <string.h>
#include "graph.h"
#include "list.h"
#include "set.h"

void graph_init(Graph *graph, int (*match) (const void *key1,const void *key2), void (*destroy) (void *data))
{
    graph->vCount = 0;
    graph->eCount = 0;
    graph->match = match;
    graph->destroy = destroy;
    list_init(&graph->adjLists, NULL);
}

void graph_destroy(Graph *graph)
{
    AdjList *adjList;
    while (list_size(&graph->adjLists) > 0) {
        if (0 == list_rem_next(&graph->adjLists, NULL, (void **)&adjList)) {
            set_destroy(&adjList->adjacent);
            if (NULL != graph->destroy) graph->destroy(adjList->vertex);
            free(adjList);
        }
    }
    list_destroy(&graph->adjLists);
    memset(graph, 0, sizeof(Graph));
}

/**
 * 插入顶点
 * @param graph
 * @param data
 * @return
 */
int graph_ins_vertex(Graph *graph, const void *data)
{
    ListElement  *element;
    AdjList *adjList;
    int result;
    /** 检测该顶点是否存在 */
    for (element = list_head(&graph->adjLists); NULL != element; element = list_next(element)) {
        if (graph->match(data, ((AdjList *)list_data(element))->vertex)) return 1;
    }
    /** 分配AdjList空间 */
    if (NULL == (adjList = (AdjList *)malloc(sizeof(AdjList)))) return -1;
    adjList->vertex = (void *) data;
    /** 初始化集合 */
    set_init(&adjList->adjacent, graph->match, NULL);
    /** 将新顶点添加到邻接表链表末尾 */
    if (0 != (result = list_ins_next(&graph->adjLists, list_tail(&graph->adjLists), adjList))) return result;
    /** 图顶点数量自增 */
    graph->vCount++;
    return 0;


}

int graph_ins_edge(Graph *graph, const void *data1, const void *data2)
{
    ListElement *element;
    int result;
    /** 检测两个元素是否存在 */
    for (element = list_head(&graph->adjLists); NULL != element; element = list_next(element)) {
        if (graph->match(data2, ((AdjList *)list_data(element))->vertex)) break;
    }
    if (NULL == element) return -1;
    for (element = list_head(&graph->adjLists); NULL != element; element = list_next(element)) {
        if (graph->match(data1, ((AdjList *)list_data(element))->vertex)) break;
    }
    if (NULL == element) return -1;
    /** 将data2插入到data1的邻接顶点集合 */
    if (0 != (result = set_insert(&((AdjList *)list_data(element))->adjacent, data2))) return result;
    /** 图边自增 */
    graph->eCount++;
    return 0;
}

int graph_rem_vertex(Graph *graph, void **data)
{
    ListElement *element, *temp, *prev;
    AdjList *adjList;
    int found;
    prev = NULL;
    found = 0;
    for (element = list_head(&graph->adjLists); NULL != element; element = list_next(element)) {
        /** 如果data在一个邻接顶点结合中, 不允许移除 */
        if (set_is_member(&((AdjList *)list_data((element)))->adjacent, *data)) return -1;

        if (graph->match(*data, ((AdjList *)list_data(element))->vertex)) {
            /** 找到对应顶点 */
            temp = element;
            found = 1;
        }
        /** 把当前节点作为上次节点, 如果继续寻找的话 */
        if (!found) prev = element;
    }
    /** 没找到相应顶点 */
    if (!found) return -1;
    /** 如果当前顶点含有邻接顶点, 不允许移除 */
    if (set_size(&((AdjList *)list_data(temp))->adjacent) > 0) return -1;
    if (0 != list_rem_next(&graph->adjLists, prev, (void **)&adjList)) return -1;
    *data = adjList->vertex;
    free(adjList);
    graph->vCount--;
    return 0;
}

int graph_rem_edge(Graph *graph, void *data1, void **data2)
{
    ListElement *element;
    /** 判断data1在不在图中 data2不用判断, 本来就是要移除的, 不在就当移除完成 */
    for (element = list_head(&graph->adjLists); element != NULL; element = list_next(element)) {
        if (graph->match(data1, ((AdjList *)list_data(element))->vertex)) break;
    }
    if (NULL == element) return -1;
    if (0 != set_remove(&((AdjList *)list_data(element))->adjacent, data2)) return -1;
    graph->eCount--;
    return 0;
}

int graph_adj_list(const Graph *graph, const void *data, AdjList **adjList)
{
    ListElement *element, *prev;
    prev = NULL;
    for (element = list_head(&graph->adjLists); element != NULL; element = list_next(element)) {
        if (graph->match(data, ((AdjList *)list_data(element))->vertex)) break;
        prev = element;
    }
    if (NULL == element) return -1;
    *adjList = list_data(element);
    return 0;
}

/**
 * 判断指定的顶点data1是否与另一个顶点data2有邻接关系
 * @param graph
 * @param data1
 * @param data2
 * @return
 */
int graph_is_adjacent(const Graph *graph, const void *data1, const void *data2)
{
    ListElement *element, *prev;
    prev = NULL;
    for (element = list_head(&graph->adjLists); element != NULL; element = list_next(element)) {
        if (graph->match(data1, ((AdjList *)list_data(element))->vertex)) break;
        prev = element;
    }
    if (NULL == element) return -1;
    return set_is_member(&((AdjList *)list_data(element))->adjacent, data2);
}

















