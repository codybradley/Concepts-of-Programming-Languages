/*Right-click on your project, select "properties",
then C/C++, then Language, and change “OpenMP Support” to ‘Yes’.*/

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

void testIDM(int m);
int **idM(int n);

char **q3(int N);
void outputCharArray(char **charArray);
float q2(int n);
int *problem1(int array[]);
void outArray(int array[]);
int intQuestion(int in[], int n);
int mahRands[100];
int threadArray[100];

int main(int argc, char **argv)
{
    /*problem1(mahRands);
    outArray(mahRands);*/

    /*int intQArray[7] = {5, 10, 8, 12, 7, 0, 0};
    printf("Result: %d", intQuestion(intQArray, 7));*/

    /*int n = 10;
    printf("n = %d: %.2f", n, q2(n));*/

    /*int N;
    printf("N = %d: " outputCharArray(q3()));*/
    printf("This program is done.\n");

    return 0;
}

/*
char **q3(int N)
{
    int arabic[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
    char roman[13][3] = {"M\0", "CM\0", "D\0", "CD\0", "C\0", "XC\0", "L\0", "XL\0", "X\0", "IX\0", "V\0", "IV\0", "I\0"};

#pragma omp parallel for shared(in, n)
    for (int i = 0; i < n; i++)
    {
        if (in[i] % 2 != 0)
            in[i] = 0; //if an element is -1, it has been filtered out
        //this only works because the elements will be added at the end
        //and 0 acts like it has been filtered out because it adds nothing
        if (in[i] != 0)
        {
            in[i] = in[i] / n;
            in[i] = in[i] % 2;
        }
    }
    // printf("Array:");
    // for (int i = 0; i < n; i++)
    // {
    //     printf(" %d", in[i]);
    // }
    // printf("\n");
    //now reduce
#pragma omp parallel for reduction(+ \
                                   : result)
    for (int i = 0; i < n; i++)
    {
        result += in[i];
    }
    return result;
}
*/

void outputCharArray(char **charArray)
{
    ;
}

float q2(int n)
{
    float finalSum = 0;
    float tSum = 0;
    int i;
#pragma omp parallel private(tSum)
    {
#pragma omp for
        for (i = 1; i <= n; i++)
        {
            tSum = (float)i / n;
#pragma omp critical
            {
                finalSum += tSum;
            }
        }
    }
    return finalSum;
}

int *problem1(int array[])
{
    //Problem 1
    int i;
    int chunk;
#pragma omp parallel
    {
        chunk = (100 / omp_get_num_threads());
#pragma omp for schedule(static, chunk)
        for (i = 0; i < 100; i++)
        {
            mahRands[i] = rand() % 100;
            //printf("%4d", omp_get_thread_num());
        }
    }
    printf("\n");
}

void outArray(int array[])
{
    for (int i = 0; i < 100; i++)
    {
        if (i % 10 == 0)
        {
            printf("\n");
        }
        printf("%4d", array[i]);
    }
}

int intQuestion(int in[], int n)
{
    /*Convert the following code to openmp parallel
    public static intquestion(int[] in, int n){
        return IntStream.range(0,n).parallel()
        .map(i->in[i])
        .filter(i->i%2==0)
        .map(i->->(i/n%2))
        .reduce(7,(a,b)->a+b);
    }
    */

    int result = 7;
#pragma omp parallel for shared(in, n)
    for (int i = 0; i < n; i++)
    {
        if (in[i] % 2 != 0)
            in[i] = 0; //if an element is -1, it has been filtered out
        //this only works because the elements will be added at the end
        //and 0 acts like it has been filtered out because it adds nothing
        if (in[i] != 0)
        {
            in[i] = in[i] / n;
            in[i] = in[i] % 2;
        }
    }
    // printf("Array:");
    // for (int i = 0; i < n; i++)
    // {
    //     printf(" %d", in[i]);
    // }
    // printf("\n");
    //now reduce
#pragma omp parallel for reduction(+ \
                                   : result)
    for (int i = 0; i < n; i++)
    {
        result += in[i];
    }
    return result;
}