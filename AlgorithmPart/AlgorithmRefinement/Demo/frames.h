//
// Created by speauty on 2021/2/4.
//

#ifndef DEMO_FRAMES_H
#define DEMO_FRAMES_H
#include "list.h"

int allocFrame(List *frames);
int freeFrame(List *frames, int frameNumber);

#endif //DEMO_FRAMES_H
