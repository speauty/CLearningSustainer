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

/**
 * 最小二乘估计
 * @param x x点集
 * @param y x对应的y点集
 * @param n 点的个数
 * @param b1 系数
 * @param b0 系数
 */
void lsqe(const double *x, const double *y, int n, double *b1, double *b0);

/**
 * 方程求解
 * 采用牛顿迭代法根据给定的初始值来计算方程f的根
 * 如果当前得到的近似值与前一轮迭代得到的近似值之差小于delta所指定的值, 则认为当前的近似值满足要求
 * @param f 需要求解的方程
 * @param g f的导函数
 * @param x x点集
 * @param n 最大迭代次数
 * @param delta 逐次逼近的差值
 * @return
 */
int root(double (*f) (double x), double (*g) (double x), double *x, int *n, double delta);













#endif //DEMO_NUMMETHS_H
