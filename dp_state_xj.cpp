// dp_state_xj.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <string.h>

typedef long long s64;
//typedef int s64;

#define S 8
////
s64 matrix[S][S];

void dfs(int j, int prevState, int currentState)
{
    if (j == 3) {
        matrix[currentState][prevState]++;
        return;
    }

    if (j + 1 <= 3) {
        dfs(j + 1, prevState << 1, currentState << 1 | 1);
        dfs(j + 1, prevState << 1 | 1, currentState << 1);
    }

    if (j + 2 <= 3) {
        dfs(j + 2, prevState << 2 | 3, currentState << 2 | 3);
    }
}

void multiply(s64 result[S][S], s64 a[S][S], s64 b[S][S])
{
    int i, j, k;
    s64 sum = 0;
    s64 temp[S][S];

    for (i = 0; i < S; i++) {
        for (j = 0; j < S; j++) {
            sum = 0;
            for (k = 0; k < S; k++) {
                sum += a[i][k] * b[k][j];
            }
            temp[i][j] = sum;
        }
    }
    memcpy(result, temp, sizeof(temp));
}

void power(s64 matrix[][S], int n)
{
    int i, j;

    s64 result[S][S];

    memset(result, 0, sizeof(result));

    // µ¥Î»¾ØÕó
    for (i = 0; i < S; i++) {
        result[i][i] = 1;
    }

    while (n) {
        if (n & 0x01) {
            multiply(result, matrix, result);
        }

        multiply(matrix, matrix, matrix);
        n >>= 1;
    }

    memcpy(matrix, result, sizeof(result));
}

int main(void)
{
#ifdef _DEBUG
    freopen("input.txt", "r", stdin);
#endif

    int n;
    s64 result[S][S];

    //// Calc ×´Ì¬×ªÒÆ¾ØÕó
    dfs(0, 0, 0);

    while (scanf("%d", &n), n != -1) {
        ////
        memcpy(result, matrix, sizeof(result));
        power(result, n);

        printf("%lld\n", result[S - 1][S - 1]);
    }
    return 0;
}
