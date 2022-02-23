/*Right-click on your project, select "properties",
then C/C++, then Language, and change “OpenMP Support” to ‘Yes’.*/

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void testIntFunc(double from, double to, double n, double (*mathFunc)(), double (*intFunc)(), double (*intgFunc)());

double int_leftrect(double from, double to, double n, double (*func)());
double int_leftrect_a(double from, double to, double n, double (*func)());
double int_leftrect_b(double from, double to, double n, double (*func)());

/* test math funcs*/
double f2(double x)
{
    return 1.0 / x;
}

double f2a(double x)
{
    return log(x);
}

double f1(double x)
{
    return x * x * x;
}

double f1a(double x)
{
    return x * x * x * x / 4.0;
}

typedef double (*pfunc)(double, double, double, double (*)());

#define INTG(F, A, B) (F((B)) - F((A)))

const int NUMTASKS = 50;

int main()
{
    printf("Numerical Integration Optimization\n");

    printf("1/x, from 1.0 to 100.0, 10B approximations (Serial)\n");
    testIntFunc(1.0, 100.0, 10000000000.0, f2, int_leftrect, f2a);
    printf("1/x, from 1.0 to 100.0, 10M approximations (Small Tasks)\n");
    testIntFunc(1.0, 100.0, 10000000.0, f2, int_leftrect_a, f2a);
    printf("1/x, from 1.0 to 100.0, 10B approximations (Large Tasks)\n");
    testIntFunc(1.0, 100.0, 10000000000.0, f2, int_leftrect_b, f2a);

    printf("x^3, from 0.0 to 1.0, 10B approximations (Serial)\n");
    testIntFunc(0.0, 1.0, 10000000000.0, f1, int_leftrect, f1a);
    printf("x^3, from 0.0 to 1.0, 10M approximations (Small Tasks)\n");
    testIntFunc(0.0, 1.0, 10000000.0, f1, int_leftrect_a, f1a);
    printf("x^3, from 0.0 to 1.0, 10B approximations (Large Tasks)\n");
    testIntFunc(0.0, 1.0, 10000000000.0, f1, int_leftrect_b, f1a);

    return 0;
}

void testIntFunc(double from, double to, double n, double (*mathFunc)(), double (*intFunc)(), double (*intgFunc)())
{
    double ic, time1, time2;
    time1 = omp_get_wtime();
    ic = intFunc(from, to, n, mathFunc);
    time2 = omp_get_wtime();
    printf("Num Integration: %7.3lf\n", ic);
    printf("Actual Value: %10.3lf\n", INTG(intgFunc, from, to));
    printf("Time Used: %13.3lf seconds\n\n", time2 - time1);
}

//regular function
double int_leftrect(double from, double to, double n, double (*func)())
{
    double h = (to - from) / n;
    double sum = 0.0, x;
    for (x = from; x <= (to - h); x += h)
        sum += func(x);
    return h * sum;
}

double int_leftrect_a(double from, double to, double n, double (*func)())
{

    double h = (to - from) / n;
    double sum = 0.0, finalSum = 0.0, x;
    double buffer;
#pragma omp parallel
    {
#pragma omp master
        {

            for (x = from; x <= (to - h); x += h)
            {
#pragma omp task shared(func, sum) private(buffer) firstprivate(x)
                {
                    buffer = func(x);
#pragma omp critical
                    {
                        sum += buffer;
                    }
                }
            }
        }
    }
    return h * sum;
}

//parallel function
double int_leftrect_b(double from, double to, double n, double (*func)())
{

    double h = (to - from) / n;
    double k = (to - from) / NUMTASKS; //interval of outter loop
    double sum = 0.0, finalSum = 0.0, x;
#pragma omp parallel
    {
#pragma omp master
        {
            for (double i = from; i <= to - h; i += k) //loops through NUMTASKS times
            {
#pragma omp task shared(from, to, h, k, func, finalSum) private(x) firstprivate(i, sum)
                {
                    for (x = i; x <= i + k; x += h)
                        sum += func(x);
#pragma omp critical
                    {
                        finalSum += sum;
                    }
                }
            }
        }
#pragma omp taskwait
    }
    return h * finalSum;
}