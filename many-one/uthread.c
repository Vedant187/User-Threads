#include "queue.h"

int first_thread = 0;
int thread_count = 1;
uthread *running = NULL;
struct itimerval timer;
struct sigaction sa;
uthread *main_th ;
thread_queue q;

// Begin the timer
void beginTimer()
{
    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = 150;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 150;
    setitimer(ITIMER_VIRTUAL, &timer, NULL);
}

void endTimer()
{
    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 0;
    setitimer(ITIMER_VIRTUAL, &timer, NULL);
}
void start_routine(void *args)
{
    beginTimer();
    uthread *newThread;
    newThread=(uthread *)args;
    newThread->ret_val=newThread->f(newThread->args);

    uthread_exit(newThread->ret_val);
  
}
void uthread_exit(void *ret_val)
{
    if (ret_val == NULL)
    {
        return;
    }
    running->status=TERMINATED;

    scheduler();
    return;
}

void scheduler()
{
    endTimer();
    // printf("Scheduler is called-->\n");
    uthread_t runnnig_tid = running->user_id;
    // printf("currently running thread is %d\n", runnnig_tid);
    uthread *previous=running;
    if (running->status == RUNNING)
    {
        running->status = READY;
    }
    enqueue(&q, running);
    // print_all_threads(&q);

    // getting the next thread details:
    // in loop, to check if the thread is already terminated or not
    uthread *temp;
    int threads_count = q.num_th;
    for (int i = 0; i < threads_count; i++)
    {
        temp = dequeue(&q);
      
        if (temp->status == READY && temp->user_id!=444)
        {
            running = temp;
            running->status = RUNNING;
            break;
        }
        else if (temp->status == TERMINATED)
        {
            enqueue(&q, temp);
        }
    }
    if (running->user_id == runnnig_tid)
    {
        running=main_th;
    }
    swapcontext(&previous->context,&running->context);
}



int uthread_join(uthread_t thread, void **retval) {

    //find the node having that particular thread id
    uthread *temp_thread;
    if(thread == running->user_id) {
        return EDEADLK;
    }
    temp_thread = get_node(&q,thread);
    if(temp_thread == NULL) {
        return ESRCH;
    }

    //check if the thread is already joined or not
    if(temp_thread->state == JOINED) {
        return EINVAL;
    }
    temp_thread->state = JOINED;

    //othewise loop until the process is terminated
    while(1) {
        // printf("waiting to terminate\n");
        if (temp_thread->status == TERMINATED) {
            break;
        }
    }
    //updating the retval  with the routine return value
    if(retval) {
        *retval = temp_thread->ret_val;
    }
    return 0;
}


void init_mainth()
{
    // initialize queue
    init(&q);
    // create main thread and initialize all its att values;
    main_th = (uthread *)malloc(sizeof(uthread));
    if (main_th == NULL)
    {
        perror("Error in malloc");
        return;
    }
    main_th->user_id = 444; //main thread has give user thread 444
    main_th->stack = NULL; // because the main thread typically runs in the process's main stack.
    main_th->stack_size = STACKSIZE;
    main_th->f = NULL;
    main_th->args = NULL;
    main_th->status = RUNNING;
    main_th->state=JOINABLE;
    main_th->wait_list = NULL;
    main_th->waitlist_size = 0;

    running = main_th; 

    // initialize the context for the main thread no need for makecontext as it is already running in processor stack
    getcontext(&main_th->context);

    sigset_t mask;
    // // // A signal mask is a set of signals that are currently blocked and cannot be delivered to the process or thread.
    sigemptyset(&mask);

    // /whenever sigvtalrm is raised, scheduler is called
    sa.sa_handler = &scheduler;
    sa.sa_flags = SA_RESTART;
    sa.sa_mask = mask;

    if (sigaction(SIGVTALRM, &sa, NULL) == -1)
    {
        printf("Sigaction Error");
    }
}

int thread_create(int *t_id, void *(*f)(void *), void *args)
{
    endTimer();
    if (!t_id || !f)
    {
        return 0;
    }

    if (!first_thread)
    {
        first_thread++;
        init_mainth(); // This will initialise first thread(main thread);
    }

    // printf("Returned in thread_create function\n");
    uthread *newThread = (uthread *)malloc(sizeof(uthread));
    if (!newThread)
    {
        perror("Malloc Failed");
    }
    newThread->stack = malloc(STACKSIZE);
    newThread->stack_size = STACKSIZE;
    newThread->args = args;
    newThread->f = f;
    newThread->status = READY; // ready
    newThread->user_id = thread_count++;
    newThread->ret_val = NULL;
    newThread->waitlist_size = 0;
    newThread->state = JOINABLE;

    // code for context switch

    // init the context
    getcontext(&newThread->context);
    // set up stack
    newThread->context.uc_stack.ss_sp = newThread->stack;
    newThread->context.uc_stack.ss_size = STACKSIZE;
    newThread->context.uc_stack.ss_flags = 0;

    // save the context
    makecontext(&(newThread->context), (void (*)(void))start_routine, 1, newThread);

    *t_id = newThread->user_id;

    enqueue(&q, newThread);
    beginTimer();
}


int uthread_cancel(uthread_t thread)
{
    uthread *thread_;
    thread_=get_node(&q, thread);
    thread_->status=TERMINATED;
    raise(SIGVTALRM);
}




// locking part

void uthread_spn_init(uthread_spnlck_t * lck){
    *lck  = false;
}

void uthread_lock(uthread_spnlck_t *lck){
    while (__sync_lock_test_and_set(lck, true))
    {
        // Spin until lock is acquired
    }
}

void uthread_unlock(uthread_spnlck_t *lck)
{
    __sync_lock_release(lck);
}



// lock usng mutex
// initialize mutex
int uthread_mutex_init(uthread_mutex_t *mutex)
{
    if (!mutex)
        return EINVAL;
    mutex->owner = NULL;
    mutex->count = 0;
    return 0;
}

// acquire mutex lock
int uthread_mutex_lock(uthread_mutex_t *mutex)
{
    if (!mutex)
        return EINVAL;
    uthread *current = running;

    // wait until the lock is released
    while (mutex->owner != NULL && mutex->owner != current)
    {
        current->status = BLOCKED_MUTEX;
        enqueue(&q, current);
        scheduler();
        dequeue(&q);
    }

    // increment the count and assign the owner
    mutex->count++;
    mutex->owner = current;

    return 0;
}

// release mutex lock
int uthread_mutex_unlock(uthread_mutex_t *mutex)
{
    if (!mutex)
        return EINVAL;

    // check if the current thread is the owner
    uthread *current = running;
    if (mutex->owner != current)
        return EINVAL;

    // decrement the count and release the lock if count reaches 0
    mutex->count--;
    if (mutex->count == 0)
        mutex->owner = NULL;

    // unblock the waiting thread
    if (!isempty(&q))
    {
        uthread *next = dequeue(&q);
        next->status = READY;
        enqueue(&q, next);
    }

    return 0;
}
