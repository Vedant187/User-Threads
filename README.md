# User-Threads
Implemented a userland one-one and many-one  multithreading library using C. Based on the functions of the p_thread library, implemented own hread_create(), thread_join(), and scheduler functions. Also included tester programs to check the performance of threading code

## These are the functions that are implemented:

* thread_create(); // provide the option to use a desired mapping.
* thread_join() ; // parent waits for thread
* thread_exit(); // thread exit
* thread_lock(); // a spinlock
* thread_unlock();  // spin-unlock
* thread_cancel() ; // cancel thread
* thread_mutex_lock(); // a mutex sleep-lock
* thread_mutex_unlock();


