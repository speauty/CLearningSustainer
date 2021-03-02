//
// Created by speauty on 2021/3/2.
//

#ifndef DEMO_NUMMETHS_H
#define DEMO_NUMMETHS_H
/** 数值计算 */


/**
 * 多项式插值
 * @param x 函数值已知的点集
 * @param fx 已知点对应的函数值
 * @param n x和fx中的元素数量
 * @param z 待求点集
 * @param pz 待求点对应的函数值
 * @param m z和pz中的元素数量
 * @return
 */
int interpol(const double *x, const double *fx, int n, double *z, double *pz, int m);

#endif //DEMO_NUMMETHS_H
