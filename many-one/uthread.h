#define _GNU_SOURCE
#include<signal.h>
#include<stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <setjmp.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <ucontext.h>
#include <stdbool.h>
#include <sched.h>

#define STACKSIZE 1024 * 1024


typedef enum {
    RUNNING ,
    READY ,
    TERMINATED ,
	BLOCKED_MUTEX
} status;

typedef enum
{
	JOINABLE,
	JOINED,
}state;


typedef pid_t uthread_t;

//structure of thread
typedef struct uthread
{
	int user_id;
	void *stack;
	int stack_size;
	void *(*f) (void *); //The fn argument is a pointer to a function that is called by the child process at the beginning of its execution
	void *args;
	void *ret_val;
	ucontext_t context;  // A jmp_buf data type that stores the thread's execution context.
	int status;
	int state;
	int *wait_list;
	int waitlist_size;
}uthread;


//library function 
int thread_create(int* t_id, void *(*f) (void *), void *arg);

int uthread_join(int t_id, void **ret_val);

void uthread_exit(void * ret_val);

int uthread_cancel(uthread_t thread);



//other functions
void beginTimer();
void endTimer();
void init_mainth();
void scheduler();
void sched_alarm();


// spinlock
typedef volatile bool uthread_spnlck_t;

void uthread_spn_init(uthread_spnlck_t * lck);
void uthread_lock(uthread_spnlck_t *lck);
void uthread_unlock(uthread_spnlck_t *lck);

// mutex
typedef struct {
	uthread * owner;
	int count;
}uthread_mutex_t;

int uthread_mutex_init(uthread_mutex_t *mutex);
int uthread_mutex_lock(uthread_mutex_t *mutex);
int uthread_mutex_unlock(uthread_mutex_t *mutex);