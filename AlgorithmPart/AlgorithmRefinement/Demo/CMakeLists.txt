cmake_minimum_required(VERSION 3.15)
project(Demo C)

set(CMAKE_C_STANDARD 99)

add_executable(Demo main.c data_structure/list.c data_structure/list.h application/frames.c application/frames.h data_structure/dlist.c data_structure/dlist.h data_structure/clist.c data_structure/clist.h application/page.c application/page.h data_structure/queue.c data_structure/queue.h data_structure/BiTree.c data_structure/BiTree.h data_structure/BSTree.c data_structure/BSTree.h data_structure/heap.c data_structure/heap.h data_structure/PriorityQueue.h search/biSearch.c search/search.h compress/compress.h compress/bit.c compress/bit.h compress/huffman.c)