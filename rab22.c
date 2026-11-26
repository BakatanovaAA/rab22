#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <locale.h>

#define N 1000
#define W 60
#define H 20

typedef double (*TFun)(double);

struct Point
{
    double x, y;
};

double Y(double x);
double V(double x);
double S(double x);
TFun vybor(int k);
int tabl(TFun f, double a, double b, double h, double* xs, double* ys);
int minmax(double* m, int n, double* mn, double* mx);
double gold_max(TFun f, double a, double b);
int monot(double* ys, int n);
int znak(double* ys, int n);
int chet(TFun f, double a, double b, double h);
int form_point(TFun f, double a, double b, double h, struct Point* p);
int form2(TFun f, double a, double b, double h, double m[][N]);
double razmah(double* m, int n);
char sym(int i, int j, int xz, int yz, int jf);

int main()
{
    setlocale(LC_ALL, "Rus");
    int menu, op, k, n, i, j, xz, yz, jf, kol;
    double a, b, h, x, y, mn, mx, hx, hy;
    double xs[N], ys[N], yg[W];
    double m[2][N];
    struct Point p[N];
    TFun f;
    FILE* fp;

    do
    {
        printf("\n1 - вычислить значение\n2 - табулировать\n3 - выполнить операцию\n0 - выход\n");
        if (scanf("%d", &menu) != 1) break;
        if (menu < 0 || menu > 3)
        {
            printf("нет такого пункта\n");
            continue;
        }
        if (menu == 0) break;

        printf("функция: 1 - S(x), 2 - V(x), 3 - Y(x)\n");
        scanf("%d", &k);
        f = vybor(k);
        if (f == NULL)
        {
            printf("нет такой функции\n");
            continue;
        }

        switch (menu)
        {
        case 1:
            printf("x = ");
            scanf("%lf", &x);
            y = f(x);
            if (isfinite(y)) printf("y = %.4lf\n", y);
            else printf("функция не определена\n");
            break;

        case 2:
            printf("a, b, h: ");
            scanf("%lf%lf%lf", &a, &b, &h);
            if (h <= 0 || a > b)
            {
                printf("ошибка\n");
                break;
            }
            n = tabl(f, a, b, h, xs, ys);
            for (i = 0; i < n; i++)
                if (isfinite(ys[i])) printf("|x = %7.2lf| y = %10.4lf|\n", xs[i], ys[i]);
                else printf("|x = %7.2lf| не опр.     |\n", xs[i]);
            break;

        case 3:
            printf("1 - записать в dat.txt\n2 - посчитать для x из dat.txt\n3 - график\n4 - максимум (золотое сечение)\n");
            printf("5 - описать функцию\n6 - массив Point\n7 - двумерный массив\n8 - размах\n");
            scanf("%d", &op);
            if (op == 2)
            {
                fp = fopen("dat.txt", "r");
                if (fp == NULL)
                {
                    printf("файл не открылся\n");
                    break;
                }
                while (fscanf(fp, "%lf%*[^\n]", &x) == 1)
                {
                    y = f(x);
                    if (isfinite(y)) printf("x = %.2lf  y = %.4lf\n", x, y);
                    else printf("x = %.2lf  не опр.\n", x);
                }
                fclose(fp);
                break;
            }
            if (op < 1 || op > 8)
            {
                printf("нет такой операции\n");
                break;
            }
            printf("a, b: ");
            scanf("%lf%lf", &a, &b);
            if (a >= b)
            {
                printf("ошибка\n");
                break;
            }
            h = 0.1;
            if (op != 3 && op != 4)
            {
                printf("h = ");
                scanf("%lf", &h);
                if (h <= 0)
                {
                    printf("ошибка\n");
                    break;
                }
            }
            n = tabl(f, a, b, h, xs, ys);

            switch (op)
            {
            case 1:
                fp = fopen("dat.txt", "w");
                for (i = 0; i < n; i++)
                    fprintf(fp, "%.2lf,%.4lf\n", xs[i], ys[i]);
                fclose(fp);
                printf("записано %d строк\n", n);
                break;

            case 3:
                hx = (b - a) / (W - 1);
                for (i = 0; i < W; i++)
                    yg[i] = f(a + i * hx);
                if (minmax(yg, W, &mn, &mx) == 0)
                {
                    printf("функция не определена на интервале\n");
                    break;
                }
                if (mn > 0) mn = 0;
                if (mx < 0) mx = 0;
                hy = (mx - mn) / (H - 1);
                if (hy == 0) hy = 1;
                yz = (int)floor(mx / hy + 0.5);
                xz = (int)floor((0. - a) / hx + 0.5);
                for (j = 0; j < H; j++)
                {
                    for (i = 0; i < W; i++)
                    {
                        jf = isfinite(yg[i]) ? (int)floor((mx - yg[i]) / hy + 0.5) : -1;
                        putchar(sym(i, j, xz, yz, jf));
                    }
                    putchar('\n');
                }
                break;

            case 4:
                x = gold_max(f, a, b);
                printf("max: x = %.4lf  y = %.4lf\n", x, f(x));
                break;

            case 5:
                k = monot(ys, n);
                if (k == 1) printf("возрастающая\n");
                else if (k == -1) printf("убывающая\n");
                else printf("не монотонная\n");
                printf(znak(ys, n) ? "знакопеременная\n" : "знак не меняет\n");
                k = chet(f, a, b, h);
                if (k == 1) printf("симметричная (четная)\n");
                else if (k == -1) printf("симметричная (нечетная)\n");
                else printf("не симметричная\n");
                break;

            case 6:
                kol = form_point(f, a, b, h, p);
                for (i = 0; i < kol; i++)
                    printf("(%.2lf; %.4lf)\n", p[i].x, p[i].y);
                break;

            case 7:
                n = form2(f, a, b, h, m);
                for (i = 0; i < n; i++) printf("%8.2lf", m[0][i]);
                printf("\n");
                for (i = 0; i < n; i++) printf("%8.3lf", m[1][i]);
                printf("\n");
                break;

            case 8:
                printf("размах по x = %.4lf\n", razmah(xs, n));
                printf("размах по y = %.4lf\n", razmah(ys, n));
                break;
            }
            break;
        }
    } while (menu != 0);

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

TFun vybor(int k)
{
    if (k == 1) return S;
    if (k == 2) return V;
    if (k == 3) return Y;
    return NULL;
}

int tabl(TFun f, double a, double b, double h, double* xs, double* ys)
{
    int n = 0;
    double x;
    for (x = a; x <= b + h / 2 && n < N; x += h)
    {
        xs[n] = x;
        ys[n] = f(x);
        n++;
    }
    return n;
}

int minmax(double* m, int n, double* mn, double* mx)
{
    int i, k = 0;
    for (i = 0; i < n; i++)
    {
        if (!isfinite(m[i])) continue;
        if (k == 0 || m[i] < *mn) *mn = m[i];
        if (k == 0 || m[i] > *mx) *mx = m[i];
        k++;
    }
    return k;
}

double gold_max(TFun f, double a, double b)
{
    double r = (sqrt(5.) - 1) / 2;
    double x1 = b - r * (b - a), x2 = a + r * (b - a);
    while (b - a > 1e-5)
    {
        if (f(x1) < f(x2))
        {
            a = x1;
            x1 = x2;
            x2 = a + r * (b - a);
        }
        else
        {
            b = x2;
            x2 = x1;
            x1 = b - r * (b - a);
        }
    }
    return (a + b) / 2;
}

int monot(double* ys, int n)
{
    int i, r = 1, u = 1;
    for (i = 1; i < n; i++)
    {
        if (!(ys[i] > ys[i - 1])) r = 0;
        if (!(ys[i] < ys[i - 1])) u = 0;
    }
    return r ? 1 : (u ? -1 : 0);
}

int znak(double* ys, int n)
{
    int i, pr = 0;
    for (i = 0; i < n; i++)
    {
        if (!isfinite(ys[i]) || ys[i] == 0) continue;
        if (pr != 0 && (ys[i] > 0) != (pr > 0)) return 1;
        pr = ys[i] > 0 ? 1 : -1;
    }
    return 0;
}

int chet(TFun f, double a, double b, double h)
{
    double x;
    int c = 1, nc = 1;
    for (x = a; x <= b + h / 2; x += h)
    {
        if (!isfinite(f(x)) || !isfinite(f(-x))) return 0;
        if (fabs(f(x) - f(-x)) > 1e-6) c = 0;
        if (fabs(f(x) + f(-x)) > 1e-6) nc = 0;
    }
    return c ? 1 : (nc ? -1 : 0);
}

int form_point(TFun f, double a, double b, double h, struct Point* p)
{
    double xs[N], ys[N];
    int i, n, k = 0;
    n = tabl(f, a, b, h, xs, ys);
    for (i = 0; i < n; i++)
        if (isfinite(ys[i]))
        {
            p[k].x = xs[i];
            p[k].y = ys[i];
            k++;
        }
    return k;
}

int form2(TFun f, double a, double b, double h, double m[][N])
{
    return tabl(f, a, b, h, m[0], m[1]);
}

double razmah(double* m, int n)
{
    double mn, mx;
    if (minmax(m, n, &mn, &mx) == 0) return 0;
    return fabs(mx - mn);
}

char sym(int i, int j, int xz, int yz, int jf)
{
    if (j == jf) return '*';
    if (i == xz && j == yz) return '+';
    if (j == yz) return '-';
    if (i == xz) return '|';
    return ' ';
}