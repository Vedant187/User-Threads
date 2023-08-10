#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "uthread.h"

#define NUM_THREADS 4
#define NUM_INCREMENTS 100000

int counter = 0;
uthread_spnlck_t mutex;

void *thread_func(void *arg) {
    int i;
    for (i = 0; i < NUM_INCREMENTS; i++) {
        uthread_lock(&mutex);
        counter++;
        uthread_unlock(&mutex);
    }
    uthread_exit(NULL);
}

int main() {
    int i;
    uthread_t threads[NUM_THREADS];

    uthread_spn_init(&mutex);

    for (i = 0; i < NUM_THREADS; i++) {
        thread_create(&threads[i], thread_func, NULL);
    }

    for (i = 0; i < NUM_THREADS; i++) {
        void *retval;
        uthread_join(threads[i], &retval);
    }
    uthread_lock(&mutex);
    printf("Final counter value: %d\n", counter);
    uthread_unlock(&mutex);

    printf("main process ended\n");
    return 0;
}