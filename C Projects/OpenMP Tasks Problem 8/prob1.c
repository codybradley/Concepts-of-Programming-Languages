#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

char **q3(int N);
void outputCharArray(char **charArray);
int *partialSum(int *input, int size);
int sumHelper(int *input, int size, int start);
void outArray(int *array, int size);
int *randArray(int n, int m);
int *evenRandom(int n);

int main(int argc, char *argv[])
{
    /*//Testing partialSum()
    int size = 5;
    int inputArray[size];
    int *result;
    for (int i = 0; i < size; i++)
    {
        inputArray[i] = i;
    }

    result = partialSum(inputArray, size);
    outArray(result, size);
    */

    //Testing randArray
    /*int *result;
    int n = 10;
    int m = 100;

    result = randArray(n, m);
    outArray(result, n);
    */

    /*int *result;
    int n = 10;

    result = evenRandom(n);
    outArray(result, n);*/

    int N = 199;
    printf("N = %d: ", N);
    outputCharArray(q3(N));

    return 0;
}

char **q3(int N)
{
    int maxArraySize = 100;
    //arbitrary limit of 100, but it won't exceed 13*4 elements if the input is under 4000
    int arabic[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
    char roman[13][3] = {"M\0", "CM\0", "D\0", "CD\0", "C\0", "XC\0", "L\0", "XL\0", "X\0", "IX\0", "V\0", "IV\0", "I\0"};
    char **result = (char **)malloc(maxArraySize * 3 * sizeof(char));

    for (int i = 0; i < maxArraySize; i++)
    {
        result[i] = '\0';
    }

    int i = 0;
#pragma omp parallel
    {
#pragma omp master
        {
            for (int j = 0; j < 13; j++)
            {
                for (; i < maxArraySize; i++)
                {
#pragma omp task shared(result, N, maxArraySize) firstprivate(i)
                    {
                        while (N >= arabic[j])
                        {
                            N -= arabic[j];
                            for (int k = 0; k < 3 && roman[j][k] != '\0'; k++)
                                result[i][k] = roman[j][k];
                        }
                    }
                }
            }
        }
#pragma omp taskwait
    }

    return result;
}

void outputCharArray(char **charArray)
{
    for (int i = 0; i < 100; i++)
        for (int j = 0; j < 3; j++)
            printf("%c", charArray[i][j]);
}

int *partialSum(int *input, int size)
{
    int *result = (int *)calloc(size, sizeof(int));
    int i = 0;
#pragma omp parallel
    {
#pragma omp master
        {
            for (; i < size; i++)
            {
#pragma omp task shared(result, input, size) firstprivate(i)
                {
                    result[i] = sumHelper(input, size, i);
                }
            }
        }
#pragma omp taskwait
    }
    return result;
}

int sumHelper(int *input, int size, int start)
{
    int result = 0;
    for (int i = start; i < size; i++)
    {
        result += input[i];
    }
    return result;
}

void outArray(int *array, int size)
{
    printf("Array:");
    for (int i = 0; i < size; i++)
    {
        printf(" %d", array[i]);
    }
    printf("\n");
}

int *randArray(int n, int m)
{
    int *result = (int *)calloc(n, sizeof(int));
    int i = 0;
#pragma omp parallel
    {
#pragma omp master
        {
            for (; i < n; i++)
            {
#pragma omp task shared(result, m) firstprivate(i)
                {
                    result[i] = (rand() % m) + 1;
                }
            }
        }
#pragma omp taskwait
    }
    return result;
}

int *evenRandom(int n)
{
    int *result = (int *)calloc(n, sizeof(int));
    int i = 0;
#pragma omp parallel
    {
#pragma omp master
        {
            for (; i < n; i++)
            {
#pragma omp task shared(result, n) firstprivate(i)
                {
                    do
                    {
                        result[i] = rand();
                    } while (result[i] % 2 != 0);
                }
            }
        }
#pragma omp taskwait
    }
    return result;
}