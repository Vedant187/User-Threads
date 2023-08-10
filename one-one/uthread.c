#include "uthread.h"
#include <sys/mman.h>
#include <errno.h>
#include <linux/futex.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <sched.h>
#include <sys/resource.h>
#include <stdio.h>

thread_list *tl;
int user_tid_cnt=1;
int first=0;
/*typedef struct uthread
{
	int user_tid; //This is the user-level thread ID, which is assigned by your multithreading library when a new thread is created. 
	pid_t kernel_tid; //This is the kernel-level thread ID, which is assigned by the operating system when a new thread is created. It is used by the operating system to manage the thread and allocate system resources such as CPU time.
	// int32_t futex; This is a Linux-specific synchronization primitive that is used to implement efficient user-level blocking. It is an integer value that is used as a synchronization variable to coordinate access to shared resources among threads.
	void *stack; //This is a pointer to the function that the thread will execute. It is a pointer to a function that takes a void* argument and returns a void* value.
	int stack_size; 
	void *(*f) (void *);// This is a pointer to the function that the thread will execute. It is a pointer to a function that takes a void* argument and returns a void* value.
	void *arg;//This is a pointer to the argument that will be passed to the thread's function when it is executed. It is a void* value that can be used to pass arbitrary data to the thread.
	void *ret_val;//This is a pointer to the value that the thread's function will return when it completes. It is a void* value that can be used to retrieve the result of the thread's computation

}uthread;*/

void init_list()
{
	tl->start=NULL;
	tl->end=NULL;
}

void insert_thread(uthread **thread)
{
	thread_node *newThNode;
	newThNode=malloc(sizeof(thread_node));
	newThNode->next=NULL;
	newThNode->thread=*thread;
	if(tl->start==NULL)
	{	
		tl->start=newThNode;
		//tl->end=newThNode;
		printf("Thread list empty\n");
		return;
	}
	
	thread_node *temp=tl->start;
	while(temp->next!=NULL)
	{
		temp=temp->next;
	}
	temp->next=newThNode;	
}

int thread_create(int* tid, void *(*f),void *arg)
{	

    if(first==0) //First thread
    { 
	first++;
	init_list();
    }	
  
    if(tid==NULL || f==NULL)
    {
        printf("Error \n");
        return(EINVAL);
    }
	
    //Allocate stack for thread
    void *stack=NULL;  
    stack = mmap(NULL, SIZESTACK, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    
    //create user thread 
    uthread *thread=NULL;
    thread=malloc(sizeof(thread));
    thread->user_tid=user_tid_cnt++;
    thread->stack=stack;
    //thread->f=; //???
    thread->stack_size=SIZESTACK;
   
    //create kernel thread with the help of clone system call
	thread->kernel_tid = clone(*f, stack + thread->stack_size, SIGCHLD | CLONE_FS | CLONE_FILES | CLONE_VM | CLONE_SIGHAND | CLONE_SYSVSEM |CLONE_PARENT_SETTID, arg);
	if(thread->kernel_tid == -1){
        perror("Error in Cloning");
        free(stack);
        free(thread);
        return -1;
    }
    
    insert_thread(&thread);
}

