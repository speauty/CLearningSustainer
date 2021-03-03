//
// Created by speauty on 2021/3/3.
//
/** 方程求解实现 */
#include <math.h>
#include "nummeths.h"

int root(double (*f) (double x), double (*g) (double x), double *x, int *n, double delta)
{
    int satisfied, i;
    i = 0;
    satisfied = 0;
    while (!satisfied && (i+1)<*n) {
        x[i+1] = x[i]-(f(x[i])/g(x[i]));
        /** 允许误差范围 */
        if (fabs(x[i+1]-x[i]) < delta) satisfied = 1;
        i++;
    }
    if (0 == i) {
        *n = 1;
    } else {
        *n = i+1;
    }

    if (satisfied) {
        return 0;
    } else {
        return -1;
    }
}