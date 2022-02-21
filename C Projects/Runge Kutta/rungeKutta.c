// C program to implement Runge
// Kutta method

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// A sample differential equation
// "dy/dx = (x - y)/2"
double dydxa(double x, double y)
{
    return (x + y - 2);
}

double dydxb(double x, double y)
{
    return (pow(y, 0.2) * pow(x, 0.1) / 2.0);
}

void testFuncs(double x0, double y0, double x, double h, double (*dydxFunc)(), double (*rungeFunc)());
double rungeKutta(double x0, double y0, double x, double h, double (*dydxFunc)());
double rungeKutta_a(double x0, double y0, double x, double h, double (*dydxFunc)());

// Driver Code
int main()
{
    double x0 = 0, y = 1, x = 2, h1 = 0.000000002, h2 = 0.00000002;

    printf("Simple Differential Equation\n1B Iterations (Serial Original)\n");
    testFuncs(x0, y, x, h1, dydxa, rungeKutta);
    printf("Simple Differential Equation\n1B Iterations (Parallel Incorrect)\n");
    testFuncs(x0, y, x, h1, dydxa, rungeKutta_a);

    printf("Complex Differential Equation\n100M Iterations (Serial Original)\n");
    testFuncs(x0, y, x, h2, dydxb, rungeKutta);
    printf("Complex Differential Equation\n100M Iterations (Parallel Incorrect)\n");
    testFuncs(x0, y, x, h2, dydxb, rungeKutta_a);

    return 0;
}

void testFuncs(double x0, double y0, double x, double h, double (*dydxFunc)(), double (*rungeFunc)())
{
    double time1, time2;
    time1 = omp_get_wtime();
    printf("y(x) = %f\n", rungeFunc(x0, y0, x, h, dydxFunc));
    time2 = omp_get_wtime();
    printf("Time Used: %.3lf seconds\n\n", time2 - time1);
}

// Serial Original
// Finds value of y for a given x
// using step size h
// and initial value y0 at x0.
double rungeKutta(double x0, double y0, double x, double h, double (*dydxFunc)())
{
    // Count number of iterations
    // using step size or
    // step height h
    int n = (int)((x - x0) / h);

    double k1, k2;

    // Iterate for number of iterations
    double y = y0;
    for (int i = 1; i <= n; i++)
    {
        // Apply Runge Kutta Formulas
        // to find next value of y
        k1 = h * dydxFunc(x0, y);
        k2 = h * dydxFunc(x0 + 0.5 * h, y + 0.5 * k1);

        // Update next value of y
        y = y + (1.0 / 6.0) * (k1 + 2 * k2);

        // Update next value of x
        x0 = x0 + h;
    }

    return y;
}

//Parallel Incorrect
double rungeKutta_a(double x0, double y0, double x, double h, double (*dydxFunc)())
{
    int n = (int)((x - x0) / h);
    double k1, k2;
    double y = y0;

#pragma omp parallel for
    for (int i = 1; i <= n; i++)
    {
        k1 = h * dydxFunc(x0, y);
        k2 = h * dydxFunc(x0 + 0.5 * h, y + 0.5 * k1);
        y = y + (1.0 / 6.0) * (k1 + 2 * k2);
        x0 = x0 + h;
    }
    return y;
}