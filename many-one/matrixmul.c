
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include "uthread.h"

#define ROWS 3
#define COLS 3
#define THREADS 3

int mat1[ROWS][COLS] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
int mat2[ROWS][COLS] = {{9, 8, 7}, {6, 5, 4}, {3, 2, 1}};
int result[ROWS][COLS];

uthread_spnlck_t mutex;

void *multiply(void *arg)
{
    int thread_id = *((int *)arg);
    int start = (thread_id * ROWS) / THREADS;
    int end = ((thread_id + 1) * ROWS) / THREADS;
    for (int i = start; i < end; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            int sum = 0;
            for (int k = 0; k < ROWS; k++)
            {
                sum += mat1[i][k] * mat2[k][j];
            }
            uthread_lock(&mutex);
            result[i][j] = sum;
            uthread_unlock(&mutex);
        }
    }
}

int main()
{
    uthread_t threads[THREADS];
    int thread_ids[THREADS];
    uthread_spn_init(&mutex);

    for (int i = 0; i < THREADS; i++)
    {
        thread_ids[i] = i;
        thread_create(&threads[i], &multiply, &thread_ids[i]);
    }

    for (int i = 0; i < THREADS; i++)
    {
        uthread_join(threads[i], NULL);
    }

    printf("Result:\n");
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            printf("%d ", result[i][j]);
        }
        printf("\n");
    }

    return 0;
}