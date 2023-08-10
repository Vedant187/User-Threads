#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "uthread.h"

void* func1(void* arg) {
    printf("Thread 1 started\n");
    int i=0;
    while(i<10000000)
    {
        // printf("%d\n",i);
        i++;
    }
    int* ret = malloc(sizeof(int));
    *ret = 10;
    printf("Thread 1 ended with return value =%d\n",*ret);

    return ret;
}

void* func2(void* arg) {
    printf("Thread 2 started\n");
    int* ret = malloc(sizeof(int));
    *ret = 20;
    int i=0;
    while(i<100000000)
    {
        i++;
    }
    printf("Thread 2 ended with return value =%d\n",*ret);

    return ret;
}

void* func3(void* arg) {
    printf("Thread 3 started\n");
    // sleep(3);
    int* ret = malloc(sizeof(int));
    *ret = 30;
    
    printf("Thread 3 ended with return value =%d\n",*ret);
    return ret;
}

int main() {
    int t1, t2, t3;
    int *ret1, *ret2, *ret3;

    thread_create(&t1, func1, NULL);
    thread_create(&t2, func2, NULL);
    thread_create(&t3, func3, NULL);

    printf("Threads created\n");

    uthread_join(t1, (void**) &ret1);
    uthread_join(t2, (void**) &ret2);
    uthread_join(t3, (void**) &ret3);

    printf("Threads joined\n");

    printf("Thread 1 returned %d\n", *ret1);
    printf("Thread 2 returned %d\n", *ret2);
    printf("Thread 3 returned %d\n", *ret3);

    free(ret1);
    free(ret2);
    free(ret3);
    printf("main process ended\n");
    return 0;
}
