#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "uthread.h"

// Define global variables
uthread_spnlck_t lock1;

// Define thread function for spinlock testing
void *spinlock_test(void *args)
{
    int id = *(int *)args;
    printf("Thread %d starting spinlock test...\n", id);

    // Acquire lock using spinlock
    uthread_lock(&lock1);
    printf("Thread %d acquired lock using spinlock\n", id);
    usleep(500000); // Sleep for 500ms to simulate work being done
    uthread_unlock(&lock1); // Release lock using spinlock
    printf("Thread %d released lock using spinlock\n", id);

    return NULL;
}



int main(int argc, char *argv[])
{
    int thread_ids[4] = {1, 2, 3, 4};
    uthread_spn_init(&lock1);
    

    // Create threads to test spinlock
    for (int i = 0; i < 4; i++)
    {
        thread_create(&thread_ids[i], spinlock_test, &thread_ids[i]);
    }

  

    // Join threads to wait for their completion
    void *ret_val;
    for (int i = 0; i < 4; i++)
    {
        uthread_join(thread_ids[i], &ret_val);
    }


    uthread_exit(ret_val);
    return 0;
}
