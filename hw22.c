/* №2 */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <locale.h>

typedef double (*TFun)(double);

double Y(double x);
double V(double x);
double S(double x);
double calculate(TFun prtf, double* x, int n);

int main()
{
    setlocale(LC_ALL, "Rus");
    double x[100], res;
    int n, i, k;
    TFun f;

    printf("функция: 1 - S(x), 2 - V(x), 3 - Y(x)\n");
    scanf("%d", &k);
    if (k == 1) f = S;
    else if (k == 2) f = V;
    else if (k == 3) f = Y;
    else
    {
        printf("нет такой функции\n");
        return 0;
    }

    printf("n = ");
    scanf("%d", &n);
    if (n < 1 || n > 100)
    {
        printf("ошибка\n");
        return 0;
    }
    printf("введите %d значений x\n", n);
    for (i = 0; i < n; i++)
        scanf("%lf", &x[i]);

    res = calculate(f, x, n);
    if (isnan(res)) printf("функция не определена ни в одной точке\n");
    else printf("max + min = %.4lf\n", res);

    return 0;
}

double Y(double x)
{
    if (x <= 0) return NAN;
    return log(fabs(sqrt(pow(x, 3))));
}

double V(double x)
{
    if (x > 1) return sqrt(tan(x * x - 1));
    else if (x >= 0) return -2 * x;
    else return exp(cos(x));
}

double S(double x)
{
    double t, s = 0;
    int k;
    if (x == -1) return NAN;
    t = (x - 1) / (x + 1);
    for (k = 1; k <= 7; k += 2)
        s += pow(t, k) / k;
    return s;
}

double calculate(TFun prtf, double* x, int n)
{
    double y, mn = 0, mx = 0;
    int i, k = 0;
    for (i = 0; i < n; i++)
    {
        y = prtf(x[i]);
        if (!isfinite(y)) continue;
        if (k == 0 || y < mn) mn = y;
        if (k == 0 || y > mx) mx = y;
        k++;
    }
    if (k == 0) return NAN;
    return mx + mn;
}