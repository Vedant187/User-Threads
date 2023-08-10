#define _GNU_SOURCE
#include <sched.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <setjmp.h>
#define SIZESTACK 1024 * 1024

//Structure of thread

typedef struct uthread
{
	int user_tid; //This is the user-level thread ID, which is assigned by your multithreading library when a new thread is created. 
	pid_t kernel_tid; //This is the kernel-level thread ID, which is assigned by the operating system when a new thread is created. It is used by the operating system to manage the thread and allocate system resources such as CPU time.
	// int32_t futex; This is a Linux-specific synchronization primitive that is used to implement efficient user-level blocking. It is an integer value that is used as a synchronization variable to coordinate access to shared resources among threads.
	void *stack; //This is a pointer to the function that the thread will execute. It is a pointer to a function that takes a void* argument and returns a void* value.
	int stack_size; 
	void *(*f) (void *);// This is a pointer to the function that the thread will execute. It is a pointer to a function that takes a void* argument and returns a void* value.
	void *arg;//This is a pointer to the argument that will be passed to the thread's function when it is executed. It is a void* value that can be used to pass arbitrary data to the thread.
	void *ret_val;//This is a pointer to the value that the thread's function will return when it completes. It is a void* value that can be used to retrieve the result of the thread's computation
}uthread;

typedef struct thread_node
{
	struct thread_node *next;
	struct uthread * thread;
}thread_node;

typedef struct thread_list
{
	thread_node *start;
	thread_node *end;
}thread_list;

int thread_create(int* tid, void *(*f),void *arg);
int thread_join(int tid,void **retval);




