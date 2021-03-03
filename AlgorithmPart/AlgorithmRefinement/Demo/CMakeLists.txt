cmake_minimum_required(VERSION 3.15)
project(Demo C)

set(CMAKE_C_STANDARD 99)

add_executable(Demo main.c data_structure/list.c data_structure/list.h application/frames.c application/frames.h data_structure/dlist.c data_structure/dlist.h data_structure/clist.c data_structure/clist.h application/page.c application/page.h data_structure/queue.c data_structure/queue.h data_structure/BiTree.c data_structure/BiTree.h data_structure/BSTree.c data_structure/BSTree.h data_structure/heap.c data_structure/heap.h data_structure/PriorityQueue.h data_structure/graph.c data_structure/graph.h application/bfs.c application/bfs.h order/issort.c order/sort.h order/qksort.c order/mgsort.c order/ctsort.c order/rxsort.c search/biSearch.c search/search.h application/spell.c application/spell.h nummeths/nummeths.h nummeths/interpol.c nummeths/lsqe.c)