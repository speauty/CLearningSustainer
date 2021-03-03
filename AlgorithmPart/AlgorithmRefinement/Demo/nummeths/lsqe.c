//
// Created by speauty on 2021/3/3.
//
/** 最小二乘估计实现 */
#include <math.h>
#include "nummeths.h"

void lsqe(const double *x, const double *y, int n, double *b1, double *b0)
{
    double sumX, sumY, sumX2, sumXY;
    int i;
    sumX = 0.0;
    sumY = 0.0;
    sumX2 = 0.0;
    sumXY = 0.0;
    /** 求和 */
    for (i = 0; i < n; ++i) {
        /** x和 */
        sumX = sumX+x[i];
        /** y和 */
        sumY = sumY+y[i];
        /** x^2和 */
        sumX2 = sumX2+pow(x[i], 2.0);
        /** x*y和 */
        sumXY = sumXY+(x[i]*y[i]);
    }
    /** 套公式 */
    *b1 = (sumXY-((sumX*sumY)/(double)n))/(sumX2-(pow(sumX, 2.0)/(
    double)n));
    *b0 = (sumY-((*b1)*sumX))/(double)n;
}
