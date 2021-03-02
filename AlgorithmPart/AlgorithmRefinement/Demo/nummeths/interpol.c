//
// Created by speauty on 2021/3/2.
//
#include <stdlib.h>
#include <string.h>
#include "nummeths.h"

/** 多项式插值 */
int interpol(const double *x, const double *fx, int n, double *z, double *pz, int m)
{
    /**
     * coeff 系数数组
     */
    double term, *table, *coeff;
    int i, j, k;

    if (NULL == (table = (double *)malloc(n* sizeof(double)))) return -1;
    if (NULL == (coeff = (double *)malloc(n* sizeof(double)))) {
        free(table);
        return -1;
    }
    coeff[0] = table[0];
    /** 这里有点不太清楚 */
    for (k = 1; k < n; ++k) {
        for (i = 0; i < n - k; ++i) {
            j = i+k;
            /** 计算差商值 */
            table[i] = (table[i+1]-table[i])/(x[j]-x[i]);
        }
        coeff[k] = table[0];
    }
    free(table);

    for (k = 0; k < m; ++k) {
        pz[k] = coeff[0];
        for (j = 0; j < n; ++j) {
            term = coeff[j];
            /** 累乘 */
            for (i = 0; i < j; ++i) term = term*(z[k]-x[i]);
            /** 累加 */
            pz[k] = pz[k] + term;
        }
    }
    free(coeff);
    return 0;
}
